/**
 * Serial command protocol of the Moore v2 firmware.
 *
 * Commands are ASCII lines terminated by CR and/or LF. Set commands are silent:
 * the board only answers queries and reports errors ("!E ...") and fault
 * changes ("!F ...") on its own, so the PC can stream speed updates without
 * waiting for replies. Every received line also feeds the watchdog.
 */
#include "protocol.h"
#include "motion.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FW_ID "MOORE-FW 2.0.0"

#define RX_SIZE  256u  /* power of two */
#define TX_SIZE  1024u /* power of two */
#define LINE_MAX 64u

static UART_HandleTypeDef *uart;

static uint8_t rx_byte;
static volatile uint8_t rx_buf[RX_SIZE];
static volatile uint32_t rx_head, rx_tail;

static uint8_t tx_buf[TX_SIZE];
static volatile uint32_t tx_head, tx_tail, tx_inflight;

static char line[LINE_MAX + 1];
static uint32_t line_len;
static bool line_overflow;
static uint32_t reported_faults;

static const char *const err_names[] = {
  [MOT_OK] = "OK",
  [MOT_ERR_AXIS] = "AXIS",
  [MOT_ERR_BUSY] = "BUSY",
  [MOT_ERR_FAULT] = "FAULT",
  [MOT_ERR_LIMIT] = "LIMIT",
  [MOT_ERR_DISABLED] = "DISABLED",
  [MOT_ERR_RANGE] = "RANGE",
  [MOT_ERR_ESTOP] = "ESTOP",
  [MOT_ERR_UNSUPPORTED] = "UNSUPPORTED",
};

/* ---- Transmit (non-blocking) --------------------------------------------- */

/* Call with interrupts disabled or from the UART ISR */
static void tx_kick(void)
{
  if (tx_inflight != 0u || tx_head == tx_tail)
    return;
  uint32_t start = tx_tail & (TX_SIZE - 1u);
  uint32_t n = tx_head - tx_tail;
  if (n > TX_SIZE - start)
    n = TX_SIZE - start; /* contiguous part only, the rest goes next */
  tx_inflight = n;
  if (HAL_UART_Transmit_IT(uart, &tx_buf[start], (uint16_t)n) != HAL_OK)
    tx_inflight = 0;
}

static void send(const char *s)
{
  size_t n = strlen(s);
  uint32_t primask = __get_PRIMASK();
  __disable_irq();
  if (n <= TX_SIZE - (tx_head - tx_tail)) { /* drop the message if the buffer is full */
    for (size_t k = 0; k < n; k++)
      tx_buf[tx_head++ & (TX_SIZE - 1u)] = (uint8_t)s[k];
  }
  tx_kick();
  __set_PRIMASK(primask);
}

static void send_error(const char *name, const char *cmd)
{
  char buf[LINE_MAX + 24];
  snprintf(buf, sizeof buf, "!E %s %s\n", name, cmd);
  send(buf);
}

/* Fixed two decimals without printf float support */
static void fmt_hz(char *out, size_t size, float hz)
{
  long c = (long)(hz * 100.0f + (hz >= 0.0f ? 0.5f : -0.5f));
  const char *sign = c < 0 ? "-" : "";
  if (c < 0)
    c = -c;
  snprintf(out, size, "%s%ld.%02ld", sign, c / 100, c % 100);
}

/* S <faults> <inputs> then <hz> <steps> <flags> for X, Y, Z */
static void send_status(void)
{
  char buf[160];
  int n = snprintf(buf, sizeof buf, "S %04lX %04lX",
                   (unsigned long)motion_faults(), (unsigned long)motion_inputs());
  for (int i = 0; i < AXIS_COUNT; i++) {
    axis_status_t st;
    char hz[16];
    motion_axis_status(i, &st);
    fmt_hz(hz, sizeof hz, st.hz);
    n += snprintf(buf + n, sizeof buf - (size_t)n, " %s %ld %u", hz, (long)st.steps, (unsigned)st.flags);
  }
  snprintf(buf + n, sizeof buf - (size_t)n, "\n");
  send(buf);
}

/* ---- Parsing ----------------------------------------------------------------- */

static void skip_spaces(char **s)
{
  while (**s == ' ' || **s == '\t')
    (*s)++;
}

static bool at_end(char *s)
{
  skip_spaces(&s);
  return *s == '\0';
}

/* Consumes an axis letter (x, y, z) only if present */
static bool parse_axis(char **s, int *axis)
{
  skip_spaces(s);
  switch (tolower((unsigned char)**s)) {
  case 'x': *axis = AXIS_X; break;
  case 'y': *axis = AXIS_Y; break;
  case 'z': *axis = AXIS_Z; break;
  default: return false;
  }
  (*s)++;
  return true;
}

static bool parse_float(char **s, float *out)
{
  char *end;
  skip_spaces(s);
  *out = strtof(*s, &end);
  if (end == *s)
    return false;
  *s = end;
  return true;
}

static bool parse_long(char **s, long *out)
{
  char *end;
  skip_spaces(s);
  *out = strtol(*s, &end, 10);
  if (end == *s)
    return false;
  *s = end;
  return true;
}

/* V <axis> <hz>  or  V <hx> <hy> <hz> */
static bool cmd_speed(char *s, mot_err_t *err)
{
  int axis;
  float v[AXIS_COUNT];

  if (parse_axis(&s, &axis)) {
    if (!parse_float(&s, &v[0]) || !at_end(s))
      return false;
    *err = motion_set_speed(axis, v[0]);
    return true;
  }
  for (int i = 0; i < AXIS_COUNT; i++)
    if (!parse_float(&s, &v[i]))
      return false;
  if (!at_end(s))
    return false;
  *err = MOT_OK;
  for (int i = 0; i < AXIS_COUNT; i++) {
    mot_err_t e = motion_set_speed(i, v[i]);
    if (*err == MOT_OK)
      *err = e;
  }
  return true;
}

static void handle_line(char *cmdline)
{
  char *s = cmdline;
  mot_err_t err = MOT_OK;
  bool ok = true;
  int axis;
  long n;
  float v;

  motion_kick_watchdog();
  skip_spaces(&s);
  char cmd = (char)toupper((unsigned char)*s);
  if (cmd != '\0')
    s++;

  switch (cmd) {
  case 'V':
    ok = cmd_speed(s, &err);
    break;
  case 'E':
    ok = parse_axis(&s, &axis) && parse_long(&s, &n) && at_end(s);
    if (ok)
      err = motion_set_windings(axis, n != 0);
    break;
  case 'C':
    ok = parse_axis(&s, &axis) && parse_long(&s, &n) && at_end(s);
    if (ok)
      err = motion_set_cs(axis, n != 0);
    break;
  case 'A':
    ok = parse_axis(&s, &axis) && parse_float(&s, &v) && at_end(s);
    if (ok)
      err = motion_set_accel(axis, v);
    break;
  case 'Z':
    ok = parse_axis(&s, &axis) && at_end(s);
    if (ok)
      err = motion_zero_steps(axis);
    break;
  case 'S':
    ok = at_end(s);
    if (ok)
      motion_stop_all();
    break;
  case 'R':
    ok = at_end(s);
    if (ok)
      err = motion_reset_faults();
    break;
  case '?':
    ok = at_end(s);
    if (ok)
      send_status();
    break;
  case 'I':
    ok = at_end(s);
    if (ok)
      send("I " FW_ID "\n");
    break;
  default:
    ok = false;
    break;
  }

  if (!ok)
    send_error("SYNTAX", cmdline);
  else if (err != MOT_OK)
    send_error(err_names[err], cmdline);
}

/* ---- Public ------------------------------------------------------------------- */

void protocol_init(UART_HandleTypeDef *huart)
{
  uart = huart;
  HAL_UART_Receive_IT(uart, &rx_byte, 1);
  send("I " FW_ID "\n"); /* tells the PC that the board (re)started */
}

void protocol_poll(void)
{
  while (rx_tail != rx_head) {
    char c = (char)rx_buf[rx_tail & (RX_SIZE - 1u)];
    rx_tail++;
    if (c == '\r' || c == '\n') {
      line[line_len] = '\0';
      if (line_overflow)
        send_error("SYNTAX", "line too long");
      else if (line_len > 0u)
        handle_line(line);
      line_len = 0;
      line_overflow = false;
    } else if ((unsigned char)c < 0x20u || c == 0x7f) {
      continue; /* ignore control characters (e.g. noise when the port opens) */
    } else if (line_len < LINE_MAX) {
      line[line_len++] = c;
    } else {
      line_overflow = true;
    }
  }

  uint32_t f = motion_faults();
  if (f != reported_faults) {
    char buf[16];
    reported_faults = f;
    snprintf(buf, sizeof buf, "!F %04lX\n", (unsigned long)f);
    send(buf);
  }
}

void protocol_uart_rx_cplt(UART_HandleTypeDef *huart)
{
  if (huart != uart)
    return;
  if (rx_head - rx_tail < RX_SIZE)
    rx_buf[rx_head++ & (RX_SIZE - 1u)] = rx_byte;
  HAL_UART_Receive_IT(uart, &rx_byte, 1);
}

void protocol_uart_tx_cplt(UART_HandleTypeDef *huart)
{
  if (huart != uart)
    return;
  tx_tail += tx_inflight;
  tx_inflight = 0;
  tx_kick();
}

void protocol_uart_error(UART_HandleTypeDef *huart)
{
  if (huart != uart)
    return;
  HAL_UART_Receive_IT(uart, &rx_byte, 1); /* re-arm after overrun / framing errors */
}
