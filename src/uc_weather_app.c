#include "board.h"

#include <cr_section_macros.h>
#include "bme280_spi.h"

int main(void) {
    SystemCoreClockUpdate();
    Board_Init();
    Board_LED_Set(0, true);

    volatile static int i = 0 ;
    while(1) {
        i++ ;
    }
    return 0 ;
}
