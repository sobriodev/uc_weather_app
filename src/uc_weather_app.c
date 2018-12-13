#include "board.h"

#include <cr_section_macros.h>
#include "bme280_spi.h"
#include "bme280.h"
#include "uart.h"
#include "ring_buffer.h"
#include "app_logic.h"
#include "commands.h"

int main(void) {
	/* Generic initialization */
    SystemCoreClockUpdate();
    Board_Init();
    Board_LED_Set(0, false);
	/* 1s interrupt */
	SysTick_Config(SystemCoreClock / 1000);

    /* UART init */
    uart_init();

    /* SPI init */
    bme280_spi conf = { LPC_SSP1, bme280_default_ssel_init() };
    bme280_spi_init(&conf);

    /* BME280 init */
    struct bme280_dev dev;
    bme280_get_default_conf(&dev);

    /* Wait until bme280 sensor respond */
    int8_t rslt = !BME280_OK;
    while (rslt != BME280_OK){
    	rslt = bme280_init(&dev);
    }

    /* Set all settings needed */
    bme280_prepare_broadcast(&dev);

    /* Clear UART receive buffer */
    uart_clear_rx();

    /* Send data frames */
    while (1) {
    	bme280_send_data(&dev);
    }

    return 0;
}
