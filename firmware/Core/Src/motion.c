/**
 * Motion control for the Moore v2 firmware.
 *
 * The 32 bit step timer generates the STEP signals: each axis uses one output
 * compare channel in toggle mode, and the compare ISR adds half a period to its
 * CCR (two toggles = one step). A 1 ms tick applies the commanded speeds (with
 * an optional acceleration limit), handles direction changes, the safety
 * inputs and the PC watchdog.
 */
#include "motion.h"
#include <math.h>
#include <stddef.h>

/* ---- Configuration ------------------------------------------------------- */

#define MAX_STEP_HZ     40000.0f /* step rate limit per axis */
#define MIN_STEP_HZ     0.5f     /* below this the step output is stopped */
#define WATCHDOG_MS     300u     /* stop everything if the PC is silent while moving */
#define DIR_SETTLE_MS   1u       /* wait after a DIR change before the first step */
#define MIN_LEAD_TICKS  100u     /* minimum distance of the next compare from CNT */

/* Safety inputs that are actually wired (mask of SAFE_*). Others are ignored. */
#define SAFETY_INPUTS_ENABLED 0u
/* NC contact to GND with pull-up: open or broken cable reads high = tripped */
#define SAFETY_ACTIVE_LEVEL   GPIO_PIN_SET

#define BTN_PRESSED_LEVEL GPIO_PIN_SET /* Nucleo B1: stops all axes while pressed */
#define BOOT_WINDINGS_ON  1

/* Output levels: adjust after checking the driver wiring */
#define XY_DIR_POS_LEVEL GPIO_PIN_RESET /* DIR level for positive speed */
#define XY_AWO_OFF_LEVEL GPIO_PIN_SET   /* AWO level that switches the windings off */
#define XY_CS_ON_LEVEL   GPIO_PIN_SET   /* CS level that selects the CS resolution */
#define Z_DIR_POS_LEVEL  GPIO_PIN_RESET /* old firmware: 'f' = RESET */
#define Z_ENA_OFF_LEVEL  GPIO_PIN_SET   /* old firmware: 'd' (windings off) = SET */

/* ---- Hardware description ------------------------------------------------ */

typedef struct {
  uint32_t channel;           /* step timer channel */
  uint32_t cc_it, cc_flag;
  GPIO_TypeDef *dir_port;
  uint16_t dir_pin;
  GPIO_PinState dir_pos_level;
  GPIO_TypeDef *off_port;     /* AWO (new drivers) or ENA (old driver) */
  uint16_t off_pin;
  GPIO_PinState off_level;
  GPIO_TypeDef *cs_port;      /* NULL: driver without CS */
  uint16_t cs_pin;
  GPIO_PinState cs_on_level;
  uint32_t lim_pos, lim_neg;  /* limit inputs blocking each direction */
} axis_hw_t;

static const axis_hw_t hw[AXIS_COUNT] = {
  [AXIS_X] = { TIM_CHANNEL_1, TIM_IT_CC1, TIM_FLAG_CC1,
               X_DIR_GPIO_Port, X_DIR_Pin, XY_DIR_POS_LEVEL,
               X_AWO_GPIO_Port, X_AWO_Pin, XY_AWO_OFF_LEVEL,
               X_CS_GPIO_Port, X_CS_Pin, XY_CS_ON_LEVEL,
               SAFE_LIM_X_POS, SAFE_LIM_X_NEG },
  [AXIS_Y] = { TIM_CHANNEL_2, TIM_IT_CC2, TIM_FLAG_CC2,
               Y_DIR_GPIO_Port, Y_DIR_Pin, XY_DIR_POS_LEVEL,
               Y_AWO_GPIO_Port, Y_AWO_Pin, XY_AWO_OFF_LEVEL,
               Y_CS_GPIO_Port, Y_CS_Pin, XY_CS_ON_LEVEL,
               SAFE_LIM_Y_POS, SAFE_LIM_Y_NEG },
  [AXIS_Z] = { TIM_CHANNEL_3, TIM_IT_CC3, TIM_FLAG_CC3,
               Z_DIR_GPIO_Port, Z_DIR_Pin, Z_DIR_POS_LEVEL,
               Z_ENA_GPIO_Port, Z_ENA_Pin, Z_ENA_OFF_LEVEL,
               NULL, 0, GPIO_PIN_SET,
               SAFE_LIM_Z_POS, SAFE_LIM_Z_NEG },
};

typedef struct {
  GPIO_TypeDef *port;
  uint16_t pin;
  uint32_t bit;
} safety_input_t;

static const safety_input_t safety_inputs[] = {
  { ESTOP_GPIO_Port,     ESTOP_Pin,     SAFE_ESTOP },
  { LIM_X_POS_GPIO_Port, LIM_X_POS_Pin, SAFE_LIM_X_POS },
  { LIM_X_NEG_GPIO_Port, LIM_X_NEG_Pin, SAFE_LIM_X_NEG },
  { LIM_Y_POS_GPIO_Port, LIM_Y_POS_Pin, SAFE_LIM_Y_POS },
  { LIM_Y_NEG_GPIO_Port, LIM_Y_NEG_Pin, SAFE_LIM_Y_NEG },
  { LIM_Z_POS_GPIO_Port, LIM_Z_POS_Pin, SAFE_LIM_Z_POS },
  { LIM_Z_NEG_GPIO_Port, LIM_Z_NEG_Pin, SAFE_LIM_Z_NEG },
  { CARY_A_GPIO_Port,    CARY_A_Pin,    SAFE_CARY_A },
  { CARY_B_GPIO_Port,    CARY_B_Pin,    SAFE_CARY_B },
};

/* ---- State ----------------------------------------------------------------- */

typedef struct {
  volatile float target_hz;   /* commanded by the PC */
  volatile float current_hz;  /* applied, written only by the tick */
  volatile uint32_t half_ticks;
  volatile int32_t steps;
  volatile int8_t dir_sign;   /* direction currently on the DIR pin: +1 / -1, 0 = unset */
  uint8_t phase;              /* step output level, 1 = high */
  uint8_t settle;
  volatile bool running;
  volatile bool windings_on;
  volatile bool cs_on;
  volatile float accel;       /* steps/s^2, 0 = no limit */
} axis_t;

static TIM_HandleTypeDef *step_tim;
static float tick_hz;
static axis_t ax[AXIS_COUNT];
static volatile uint32_t faults;
static volatile uint32_t inputs;
static uint32_t prev_inputs;
static volatile uint32_t watchdog_ms;
static volatile bool stop_request;
static uint32_t led_ms;

/* ---- Step output ------------------------------------------------------------ */

static void oc_mode(uint32_t channel, uint32_t mode)
{
  TIM_TypeDef *t = step_tim->Instance;
  switch (channel) {
  case TIM_CHANNEL_1: MODIFY_REG(t->CCMR1, TIM_CCMR1_OC1M, mode); break;
  case TIM_CHANNEL_2: MODIFY_REG(t->CCMR1, TIM_CCMR1_OC2M, mode << 8U); break;
  case TIM_CHANNEL_3: MODIFY_REG(t->CCMR2, TIM_CCMR2_OC3M, mode); break;
  default: break;
  }
}

static void step_start(int i)
{
  const axis_hw_t *h = &hw[i];
  __HAL_TIM_DISABLE_IT(step_tim, h->cc_it);
  ax[i].phase = 0;
  oc_mode(h->channel, TIM_OCMODE_TOGGLE);
  __HAL_TIM_SET_COMPARE(step_tim, h->channel, __HAL_TIM_GET_COUNTER(step_tim) + MIN_LEAD_TICKS);
  __HAL_TIM_CLEAR_FLAG(step_tim, h->cc_flag);
  __HAL_TIM_ENABLE_IT(step_tim, h->cc_it);
  ax[i].running = true;
}

/* Immediate stop: the output is forced low, a pulse in progress is cut short */
static void step_stop(int i)
{
  const axis_hw_t *h = &hw[i];
  __HAL_TIM_DISABLE_IT(step_tim, h->cc_it);
  oc_mode(h->channel, TIM_OCMODE_FORCED_INACTIVE);
  ax[i].phase = 0;
  ax[i].running = false;
}

static void stop_all_now(void)
{
  for (int i = 0; i < AXIS_COUNT; i++) {
    ax[i].target_hz = 0.0f;
    ax[i].current_hz = 0.0f;
    if (ax[i].running)
      step_stop(i);
  }
}

void motion_on_compare(TIM_HandleTypeDef *htim)
{
  int i;
  if (htim != step_tim)
    return;
  switch (htim->Channel) {
  case HAL_TIM_ACTIVE_CHANNEL_1: i = AXIS_X; break;
  case HAL_TIM_ACTIVE_CHANNEL_2: i = AXIS_Y; break;
  case HAL_TIM_ACTIVE_CHANNEL_3: i = AXIS_Z; break;
  default: return;
  }

  axis_t *a = &ax[i];
  a->phase ^= 1u;
  if (a->phase)
    a->steps += a->dir_sign; /* count on the rising edge */

  uint32_t ch = hw[i].channel;
  uint32_t next = __HAL_TIM_GET_COMPARE(htim, ch) + a->half_ticks;
  uint32_t now = __HAL_TIM_GET_COUNTER(htim);
  if ((int32_t)(next - now) < (int32_t)MIN_LEAD_TICKS)
    next = now + MIN_LEAD_TICKS; /* late ISR: never schedule in the past */
  __HAL_TIM_SET_COMPARE(htim, ch, next);
}

/* ---- Control tick ----------------------------------------------------------- */

static bool axis_moving(int i)
{
  return ax[i].running || ax[i].target_hz != 0.0f || ax[i].current_hz != 0.0f;
}

static bool any_axis_moving(void)
{
  for (int i = 0; i < AXIS_COUNT; i++)
    if (axis_moving(i))
      return true;
  return false;
}

static uint32_t read_safety_inputs(void)
{
  uint32_t in = 0;
  for (size_t k = 0; k < sizeof safety_inputs / sizeof safety_inputs[0]; k++)
    if (HAL_GPIO_ReadPin(safety_inputs[k].port, safety_inputs[k].pin) == SAFETY_ACTIVE_LEVEL)
      in |= safety_inputs[k].bit;
  return in & SAFETY_INPUTS_ENABLED;
}

static bool blocked_by_limit(int i, float hz)
{
  return (hz > 0.0f && (inputs & hw[i].lim_pos)) || (hz < 0.0f && (inputs & hw[i].lim_neg));
}

static void update_axis(int i)
{
  axis_t *a = &ax[i];
  const axis_hw_t *h = &hw[i];
  float tgt = a->target_hz;

  if (faults != 0u || !a->windings_on || blocked_by_limit(i, tgt)) {
    tgt = 0.0f;
    a->target_hz = 0.0f;
  }

  /* reversing goes through zero so DIR only changes while stopped */
  float cur = a->current_hz;
  if (cur != 0.0f && tgt != 0.0f && (cur > 0.0f) != (tgt > 0.0f))
    tgt = 0.0f;

  if (a->accel > 0.0f) {
    float dv = a->accel * 0.001f;
    if (tgt > cur + dv)
      cur += dv;
    else if (tgt < cur - dv)
      cur -= dv;
    else
      cur = tgt;
  } else {
    cur = tgt;
  }
  a->current_hz = cur;

  if (fabsf(cur) < MIN_STEP_HZ) {
    if (a->running)
      step_stop(i);
    return;
  }

  if (!a->running) {
    int8_t sign = cur > 0.0f ? 1 : -1;
    if (a->dir_sign != sign) {
      GPIO_PinState neg = h->dir_pos_level == GPIO_PIN_SET ? GPIO_PIN_RESET : GPIO_PIN_SET;
      HAL_GPIO_WritePin(h->dir_port, h->dir_pin, sign > 0 ? h->dir_pos_level : neg);
      a->dir_sign = sign;
      a->settle = DIR_SETTLE_MS;
      return;
    }
    if (a->settle) {
      a->settle--;
      return;
    }
  }

  uint32_t half = (uint32_t)(tick_hz / (2.0f * fabsf(cur)));
  a->half_ticks = half;
  if (!a->running) {
    step_start(i);
    return;
  }

  /* speeding up: don't wait for an edge scheduled with the old, longer period */
  uint32_t primask = __get_PRIMASK();
  __disable_irq();
  uint32_t now = __HAL_TIM_GET_COUNTER(step_tim);
  uint32_t pending = __HAL_TIM_GET_COMPARE(step_tim, h->channel) - now;
  if (pending > half && pending < 0x80000000u)
    __HAL_TIM_SET_COMPARE(step_tim, h->channel, now + half);
  __set_PRIMASK(primask);
}

static void update_led(void)
{
  uint32_t period = faults ? 100u : (any_axis_moving() ? 250u : 0u);
  if (period == 0u) {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    led_ms = 0;
  } else if (++led_ms >= period) {
    led_ms = 0;
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
  }
}

void motion_tick_1ms(void)
{
  /* safety inputs latch a fault on the transition to tripped */
  uint32_t in = read_safety_inputs();
  uint32_t tripped = in & ~prev_inputs;
  prev_inputs = in;
  inputs = in;
  if (tripped) {
    faults |= tripped;
    stop_all_now();
  }

  if (any_axis_moving()) {
    if (++watchdog_ms >= WATCHDOG_MS) {
      faults |= FAULT_WATCHDOG;
      stop_all_now();
    }
  } else {
    watchdog_ms = 0;
  }

  if (stop_request || HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == BTN_PRESSED_LEVEL) {
    stop_request = false;
    stop_all_now();
  }

  for (int i = 0; i < AXIS_COUNT; i++)
    update_axis(i);

  update_led();
}

/* ---- Init and commands ------------------------------------------------------- */

void motion_init(TIM_HandleTypeDef *tim)
{
  step_tim = tim;

  uint32_t clk = HAL_RCC_GetPCLK1Freq();
  if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_HCLK_DIV1)
    clk *= 2u; /* timer clock is doubled when APB1 is divided */
  tick_hz = (float)clk / (float)(step_tim->Init.Prescaler + 1u);

  for (int i = 0; i < AXIS_COUNT; i++) {
    const axis_hw_t *h = &hw[i];
    oc_mode(h->channel, TIM_OCMODE_FORCED_INACTIVE);
    HAL_TIM_OC_Start(step_tim, h->channel); /* output enabled, no interrupt yet */

    ax[i].windings_on = BOOT_WINDINGS_ON;
    GPIO_PinState on = h->off_level == GPIO_PIN_SET ? GPIO_PIN_RESET : GPIO_PIN_SET;
    HAL_GPIO_WritePin(h->off_port, h->off_pin, ax[i].windings_on ? on : h->off_level);

    ax[i].cs_on = false;
    if (h->cs_port) {
      GPIO_PinState off = h->cs_on_level == GPIO_PIN_SET ? GPIO_PIN_RESET : GPIO_PIN_SET;
      HAL_GPIO_WritePin(h->cs_port, h->cs_pin, off);
    }
  }
  prev_inputs = 0; /* an input already tripped at boot latches a fault */
}

static bool valid_axis(int axis)
{
  return axis >= 0 && axis < AXIS_COUNT;
}

mot_err_t motion_set_speed(int axis, float hz)
{
  if (!valid_axis(axis))
    return MOT_ERR_AXIS;
  if (!isfinite(hz) || fabsf(hz) > MAX_STEP_HZ)
    return MOT_ERR_RANGE;
  if (hz != 0.0f) {
    if (inputs & SAFE_ESTOP)
      return MOT_ERR_ESTOP;
    if (faults)
      return MOT_ERR_FAULT;
    if (!ax[axis].windings_on)
      return MOT_ERR_DISABLED;
    if (blocked_by_limit(axis, hz))
      return MOT_ERR_LIMIT;
  }
  ax[axis].target_hz = hz;
  return MOT_OK;
}

mot_err_t motion_set_windings(int axis, bool on)
{
  if (!valid_axis(axis))
    return MOT_ERR_AXIS;
  if (axis_moving(axis))
    return MOT_ERR_BUSY;
  const axis_hw_t *h = &hw[axis];
  GPIO_PinState on_level = h->off_level == GPIO_PIN_SET ? GPIO_PIN_RESET : GPIO_PIN_SET;
  HAL_GPIO_WritePin(h->off_port, h->off_pin, on ? on_level : h->off_level);
  ax[axis].windings_on = on;
  return MOT_OK;
}

mot_err_t motion_set_cs(int axis, bool on)
{
  if (!valid_axis(axis))
    return MOT_ERR_AXIS;
  const axis_hw_t *h = &hw[axis];
  if (!h->cs_port)
    return MOT_ERR_UNSUPPORTED;
  if (axis_moving(axis))
    return MOT_ERR_BUSY; /* the step size would change in the middle of a move */
  GPIO_PinState off = h->cs_on_level == GPIO_PIN_SET ? GPIO_PIN_RESET : GPIO_PIN_SET;
  HAL_GPIO_WritePin(h->cs_port, h->cs_pin, on ? h->cs_on_level : off);
  ax[axis].cs_on = on;
  return MOT_OK;
}

mot_err_t motion_set_accel(int axis, float steps_per_s2)
{
  if (!valid_axis(axis))
    return MOT_ERR_AXIS;
  if (!isfinite(steps_per_s2) || steps_per_s2 < 0.0f)
    return MOT_ERR_RANGE;
  ax[axis].accel = steps_per_s2;
  return MOT_OK;
}

void motion_stop_all(void)
{
  for (int i = 0; i < AXIS_COUNT; i++)
    ax[i].target_hz = 0.0f;
  stop_request = true; /* immediate stop at the next tick, without ramp */
}

mot_err_t motion_reset_faults(void)
{
  if (inputs & SAFE_ESTOP)
    return MOT_ERR_ESTOP;
  faults = 0;
  return MOT_OK;
}

mot_err_t motion_zero_steps(int axis)
{
  if (!valid_axis(axis))
    return MOT_ERR_AXIS;
  ax[axis].steps = 0;
  return MOT_OK;
}

void motion_kick_watchdog(void)
{
  watchdog_ms = 0;
}

uint32_t motion_faults(void)
{
  return faults;
}

uint32_t motion_inputs(void)
{
  return inputs;
}

void motion_axis_status(int axis, axis_status_t *st)
{
  const axis_t *a = &ax[axis];
  st->hz = a->current_hz;
  st->steps = a->steps;
  st->flags = (a->windings_on ? AXF_WINDINGS : 0u) | (a->cs_on ? AXF_CS : 0u) |
              (a->running ? AXF_MOVING : 0u);
}
