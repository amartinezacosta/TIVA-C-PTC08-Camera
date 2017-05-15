#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "buffer.h"

#define UART5_BUFFER_SIZE   1024

void UART5_Init(void);
uint32_t UART5_ReadBytes(uint8_t *bytes, uint32_t size);
bool UART5_WriteBytes(uint8_t *bytes, uint32_t size);
uint32_t UART5_Available(void);

#endif /* UART_H_ */
