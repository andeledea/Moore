/**
 * Serial command protocol of the Moore v2 firmware (ASCII lines over the
 * ST-LINK virtual COM port). See firmware/README.md for the command list.
 */
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "main.h"

/** Starts reception and sends the identification line. */
void protocol_init(UART_HandleTypeDef *huart);

/** Parses received lines and reports fault changes. Call from the main loop. */
void protocol_poll(void);

void protocol_uart_rx_cplt(UART_HandleTypeDef *huart);
void protocol_uart_tx_cplt(UART_HandleTypeDef *huart);
void protocol_uart_error(UART_HandleTypeDef *huart);

#endif /* PROTOCOL_H */
