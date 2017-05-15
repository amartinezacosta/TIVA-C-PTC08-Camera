#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "uart.h"
#include "PTC08.h"
#include "timer.h"
#include "utils/uartstdio.h"

#define FRAME_BUFFER_SIZE   133
#define READ_SIZE           FRAME_BUFFER_SIZE - 5


void
InitConsole(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTStdioConfig(0, 115200, 16000000);
}

void UART0_WriteBytes(uint8_t *bytes, uint32_t size){
    uint32_t i;
    for(i = 0; i < size; i++){
        UARTCharPut(UART0_BASE, bytes[i]);
    }
}


int main(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    UART5_Init();
    TIMER0_init();
    InitConsole();

    uint32_t FrameSize = 0;
    uint16_t FrameAddress = 0;
    uint16_t ReadSize = READ_SIZE;
    uint8_t TempFrameBuffer[FRAME_BUFFER_SIZE] = { 0 };
    uint16_t RequestedBytes;
    uint8_t ImageSize;
    uint8_t *ResetInfo;
    char input[16];

    UARTprintf("Press enter to continue\n");
    UARTgets(input, 16);

    if((ResetInfo = PTC08_Reset(DEFAULT_SERIAL_NUMBER)) == NULL){
        //UARTprintf("Error reseting the camera\n");
        while(1);
    }

    //UARTprintf("Reset Info: %s", ResetInfo);

    if(!PTC08_ReadData(DEFAULT_SERIAL_NUMBER, I2C_EEPROM, 1, 0x0019, &ImageSize)){
        //UARTprintf("Error: Reading Image Size\n");
        while(1);
    }

    switch(ImageSize){
    case IMAGE_640x480:
        //UARTprintf("Image Size is 640x480\n");
        break;
    case IMAGE_320x240:
        //UARTprintf("Image Size is 320x240\n");
        break;
    case IMAGE_160x120:
        //UARTprintf("Image Size is 160x120\n");
        break;
    default:
        //UARTprintf("Warning: Could not read Image Size properly\n");
        break;
    }


    //UARTprintf("Taking Picture in 3 Seconds\n");
    SysCtlDelay(15000000);

    if(!PTC08_FBuffCtrl(DEFAULT_SERIAL_NUMBER, STOP_CURRENT_FRAME)){
        //UARTprintf("Error: Stopping current frame\n");
        while(1);
    }

    FrameSize = PTC08_GetFBuffLen(DEFAULT_SERIAL_NUMBER, CURRENT_FRAME);
    if(FrameSize == 0){
        //UARTprintf("Error: frame size is 0\n");
        while(1);
    }

    UARTprintf("%i\n", FrameSize);
    //SysCtlDelay(15000000);

    //UARTprintf("Reading Picture into Tiva's C memory\n");
    while(FrameSize > 0){
        RequestedBytes = MIN(ReadSize, FrameSize);

        if(!PTC08_ReadFBuff(DEFAULT_SERIAL_NUMBER, CURRENT_FRAME, MCU_MODE, FrameAddress, RequestedBytes, 10, TempFrameBuffer)){
            //UARTprintf("\nError: Buffer Read Error\n");
            while(1);
        }

        UART0_WriteBytes(TempFrameBuffer, READ_SIZE);

        FrameSize -= RequestedBytes;
        FrameAddress += RequestedBytes;
    }

    //UARTprintf("\nDone Reading Picture\n");
    while(1){

	}
}
