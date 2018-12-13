/*
 * uart.h
 *
 *  Created on: 7 gru 2018
 *      Author: sobriodev
 */

#ifndef UART_H_
#define UART_H_

#define UART_SRB_SIZE 32
#define UART_RRB_SIZE 4

/*!
 * @brief UART configuration
 * @return Nothing
 */
void uart_init(void);

/*!
 * @brief Clear UART RX buffer
 * @return Nothing
 */
void uart_clear_rx(void);

#endif /* UART_H_ */
