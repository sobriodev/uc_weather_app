/*
 * sensor.h
 *
 *  Created on: 11 gru 2018
 *      Author: sobriodev
 */

#ifndef APP_LOGIC_H_
#define APP_LOGIC_H_

#include "bme280.h"

/*!
 * @brief Get default configuration for bme280 sensor
 * @param[in] dev : bme280 configuration struct
 * @return Nothing
 */
void bme280_get_default_conf(struct bme280_dev *dev);

/**
 * @brief Set all settings and wake up sensor
 * @param[in] dev : bme280 configuration struct
 * @return Nothing
 */
void bme280_prepare_broadcast(struct bme280_dev *dev);

/**
 * @brief Send read data using UART
 * @return Nothing
 */
void bme280_send_data(struct bme280_dev *dev);

/* CALLBACK FUNCTIONS */

/*!
 * @brief Set delay between data readouts
 * @param[in] delay : Delay in milliseconds
 * @return Nothing
 */
void bme280_set_delay(uint32_t delay);

/*!
 * @brief Set 100ms delay
 * @return Nothing
 */
inline static void bme280_set_delay_100_ms(void)
{
	bme280_set_delay(100);
}

/*!
 * @brief Set 1s delay
 * @return Nothing
 */
inline static void bme280_set_delay_1_s(void)
{
	bme280_set_delay(1000);
}

/*!
 * @brief Set 10s delay
 * @return Nothing
 */
inline static void bme280_set_delay_10_s(void)
{
	bme280_set_delay(10000);
}

/**
 * @brief Delay function
 * @param[in] period : Time period in milliseconds
 */
void bme280_delay_ms(uint32_t period);

#endif /* APP_LOGIC_H_ */
