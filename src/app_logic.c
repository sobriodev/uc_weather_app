/*
 * app_logic.c
 *
 *  Created on: 11 gru 2018
 *      Author: sobriodev
 */

#include "chip.h"
#include "app_logic.h"
#include "bme280_spi.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"

/* ------------------------------------------------------------------------------------------- */
/* ----------------------------- Private functions and variables ----------------------------- */
/* ------------------------------------------------------------------------------------------- */

/*!
 * @brief Structure storing sensor readout data
 */
static struct bme280_data comp_data;

/*!
 * @brief Buffer used for string formatting
 */
static char sbuffer[UART_SRB_SIZE];

/*!
 * @brief Transmit/receive UART rings
 */
extern RINGBUFF_T txring, rxring;

/*!
 * @brief Timer ticks count
 */
volatile uint32_t timer_count;

/*!
 * @brief Time delay between data readouts
 */
static volatile uint32_t read_delay = 100;

/*!
 * @brief Reset flag. If true is written current delay is canceled
 */
static volatile bool reset = 0;

/**
 * @brief Systick Handler for using time delay
 * @return Nothing
 */
void SysTick_Handler(void)
{
	if (timer_count) {
		timer_count--;
	}
}

/* ------------------------------------------------------------------------------------------- */
/* -------------------------------------- API functions -------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void bme280_get_default_conf(struct bme280_dev *dev)
{
    dev->dev_id = 0;
    dev->intf = BME280_SPI_INTF;
    dev->read = bme280_spi_read;
    dev->write = bme280_spi_write;
    dev->delay_ms = bme280_delay_ms;
}

void bme280_prepare_broadcast(struct bme280_dev *dev)
{
	uint8_t settings_sel;
	dev->settings.osr_h = BME280_OVERSAMPLING_1X;
	dev->settings.osr_p = BME280_OVERSAMPLING_16X;
	dev->settings.osr_t = BME280_OVERSAMPLING_2X;
	dev->settings.filter = BME280_FILTER_COEFF_16;
	dev->settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

	settings_sel = BME280_OSR_PRESS_SEL;
	settings_sel |= BME280_OSR_TEMP_SEL;
	settings_sel |= BME280_OSR_HUM_SEL;
	settings_sel |= BME280_STANDBY_SEL;
	settings_sel |= BME280_FILTER_SEL;
	bme280_set_sensor_settings(settings_sel, dev);
	bme280_set_sensor_mode(BME280_NORMAL_MODE, dev);
}

void bme280_send_data(struct bme280_dev *dev)
{
	/* Wait for the data readout */
	bme280_delay_ms(read_delay);

	/* Send temperature frame */
	bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
	sprintf(sbuffer, "[T %0.2f]", comp_data.temperature);
	Chip_UART_SendRB(LPC_USART, &txring, sbuffer, strlen(sbuffer));

	/* Wait for bluetooth readout and send humidity frame */
	bme280_delay_ms(50);
	sprintf(sbuffer, "[H %0.2f]", comp_data.humidity);
	Chip_UART_SendRB(LPC_USART, &txring, sbuffer, strlen(sbuffer));

	/* Wait for bluetooth readout and send pressure frame */
	bme280_delay_ms(50);
	sprintf(sbuffer, "[P %0.2f]", comp_data.pressure);
	Chip_UART_SendRB(LPC_USART, &txring, sbuffer, strlen(sbuffer));
}

void bme280_set_delay(uint32_t delay)
{
	read_delay = delay;
	/* Set reset flag which cancels current delay */
	reset = true;
}

void bme280_delay_ms(uint32_t period)
{
	/* Clear reset flag */
	reset = false;
	timer_count = period;
	/* Wait until timer count to 0 or new value is set */
	while (timer_count && !reset);
}



