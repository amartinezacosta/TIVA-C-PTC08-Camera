#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "uart.h"
#include "PTC08.h"
#include "timer.h"

int main(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    UART5_Init();
    TIMER0_init();

    uint32_t lenght;

    PTC08_FBuffCtrl(DEFAULT_SERIAL_NUMBER, 0x00);
    PTC08_GetFBuffLen(DEFAULT_SERIAL_NUMBER, 0x00, &lenght);

    PTC08_ReadFBuff(DEFAULT_SERIAL_NUMBER, 0x00, 0x0F, 0, 32, 3000);

    while(1){

	}
}
