/*
 * uart.c
 *
 *  Created on: 7 gru 2018
 *      Author: sobriodev
 */

#include "board.h"
#include "uart.h"
#include "commands.h"

/* ------------------------------------------------------------------------------------------- */
/* ----------------------------- Private functions and variables ----------------------------- */
/* ------------------------------------------------------------------------------------------- */

/*!
 * @brief Transmit/receive UART rings
 */
RINGBUFF_T txring, rxring;

/*!
 * @brief Transmit/receive/command buffers
 */
static uint8_t rxbuff[UART_RRB_SIZE], txbuff[UART_SRB_SIZE], commandbuff[UART_RRB_SIZE];

/*!
 * @brief Handle transmit/receive data
 * @return Nothing
 */
void UART_IRQHandler(void)
{
	Chip_UART_IRQRBHandler(LPC_USART, &rxring, &txring);
	if (RingBuffer_GetCount(&rxring) == UART_RRB_SIZE) {
		RingBuffer_PopMult(&rxring, commandbuff, UART_RRB_SIZE);
		uart_clear_rx();
		invoke_cmd((char *) commandbuff);
	}
}

/* ------------------------------------------------------------------------------------------- */
/* -------------------------------------- API functions -------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void uart_init(void)
{
	Chip_UART_Init(LPC_USART);
	Chip_UART_SetBaud(LPC_USART, 9600);
	Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(LPC_USART);
	RingBuffer_Init(&rxring, rxbuff, 1, UART_RRB_SIZE);
	RingBuffer_Init(&txring, txbuff, 1, UART_SRB_SIZE);
	Chip_UART_IntEnable(LPC_USART, (UART_IER_RBRINT | UART_IER_RLSINT));
	NVIC_SetPriority(UART0_IRQn, 1);
	NVIC_EnableIRQ(UART0_IRQn);
}

void uart_clear_rx(void)
{
	while (RingBuffer_GetFree(&rxring) != UART_RRB_SIZE) {
		RingBuffer_Pop(&rxring, commandbuff);
	}
}
