#include "uart.h"


Buffer  UART5_Buffer;
uint8_t UART5_Data[UART5_BUFFER_SIZE];

void UART5_Init(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	GPIOPinConfigure(GPIO_PE4_U5RX);
	GPIOPinConfigure(GPIO_PE5_U5TX);
	GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), 38400,
						(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	Buffer_Init(&UART5_Buffer , UART5_Data, UART5_BUFFER_SIZE);

	IntMasterEnable();
	IntEnable(INT_UART5);
	UARTIntEnable(UART5_BASE, UART_INT_RX | UART_INT_RT);
}

bool UART5_WriteBytes(uint8_t *bytes, uint32_t size){
	uint32_t i;
	for(i = 0; i < size; i++){
		UARTCharPut(UART5_BASE, bytes[i]);
	}

	return true;
}

uint32_t UART5_Available(void){
    return Buffer_Available(&UART5_Buffer);
}


uint32_t UART5_ReadBytes(uint8_t *data, uint32_t size){
    uint32_t bytesRead;
    Buffer_Read(&UART5_Buffer, data, size, &bytesRead);

    return bytesRead;
}

void UART5IntHandler(void)
{
    uint32_t ui32Status;
    int8_t cChar;
    int32_t i32Char;
    uint32_t bytesWritten;

    ui32Status = UARTIntStatus(UART5_BASE, true);
    UARTIntClear(UART5_BASE, ui32Status);

    while(UARTCharsAvail(UART5_BASE))
    {
        i32Char =  UARTCharGetNonBlocking(UART5_BASE);
        cChar = (unsigned char)(i32Char & 0xFF);
        Buffer_Write(&UART5_Buffer, (uint8_t*)&cChar, 1, &bytesWritten);
    }
}
