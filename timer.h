#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

void TIMER0_init(void);
void TIMER0_SetTimer(uint32_t delay);
bool TIMER0_CheckTimeout(void);
void TIMER0_DisableTimer(void);


#endif /* TIMER_H_ */
