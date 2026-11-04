/**
 * Motion control for the Moore v2 firmware: step generation, speed ramps,
 * safety inputs and watchdog. See firmware/README.md for pinout and protocol.
 */
#ifndef MOTION_H
#define MOTION_H

#include <stdbool.h>
#include <stdint.h>
#include "main.h"

#define AXIS_COUNT 3
enum { AXIS_X = 0, AXIS_Y = 1, AXIS_Z = 2 };

/* Safety inputs and faults share the same bit positions */
#define SAFE_ESTOP      (1u << 0)
#define SAFE_LIM_X_POS  (1u << 1)
#define SAFE_LIM_X_NEG  (1u << 2)
#define SAFE_LIM_Y_POS  (1u << 3)
#define SAFE_LIM_Y_NEG  (1u << 4)
#define SAFE_LIM_Z_POS  (1u << 5)
#define SAFE_LIM_Z_NEG  (1u << 6)
#define SAFE_CARY_A     (1u << 7)
#define SAFE_CARY_B     (1u << 8)
#define FAULT_WATCHDOG  (1u << 9)

/* Per-axis status flags */
#define AXF_WINDINGS (1u << 0) /* windings on (motor holding) */
#define AXF_CS       (1u << 1) /* CS resolution selected */
#define AXF_MOVING   (1u << 2) /* step output running */

typedef enum {
  MOT_OK = 0,
  MOT_ERR_AXIS,
  MOT_ERR_BUSY,
  MOT_ERR_FAULT,
  MOT_ERR_LIMIT,
  MOT_ERR_DISABLED,
  MOT_ERR_RANGE,
  MOT_ERR_ESTOP,
  MOT_ERR_UNSUPPORTED,
} mot_err_t;

typedef struct {
  float hz;       /* current step rate, signed */
  int32_t steps;  /* step counter, signed */
  uint8_t flags;  /* AXF_* */
} axis_status_t;

/** Configures the outputs and starts the step timer (channels idle). */
void motion_init(TIM_HandleTypeDef *step_tim);

/** 1 ms control tick: ramps, direction changes, safety, watchdog, LED. */
void motion_tick_1ms(void);

/** Output compare ISR of the step timer. */
void motion_on_compare(TIM_HandleTypeDef *htim);

mot_err_t motion_set_speed(int axis, float hz);
mot_err_t motion_set_windings(int axis, bool on);
mot_err_t motion_set_cs(int axis, bool on);
mot_err_t motion_set_accel(int axis, float steps_per_s2);
void motion_stop_all(void);
mot_err_t motion_reset_faults(void);
mot_err_t motion_zero_steps(int axis);
void motion_kick_watchdog(void);

uint32_t motion_faults(void);
uint32_t motion_inputs(void);
void motion_axis_status(int axis, axis_status_t *st);

#endif /* MOTION_H */
