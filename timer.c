#include "timer.h"

volatile bool timeout;

void TIMER0_init(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT_UP);

    TimerDisable(TIMER0_BASE, TIMER_A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER0A);

    timeout = false;
}

void TIMER0_SetTimer(uint32_t delay){
    TimerLoadSet(TIMER0_BASE, TIMER_A, delay);
    TimerEnable(TIMER0_BASE, TIMER_A);
    timeout = false;
}

void TIMER0_DisableTimer(void){
    TimerDisable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

bool TIMER0_CheckTimeout(void){
    return timeout;
}


void TIMER0IntHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    timeout = true;
}
