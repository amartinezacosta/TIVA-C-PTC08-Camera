#include "PTC08.h"

static void PTC08_SendCommand(uint8_t cmd, uint8_t SerialNumber, uint8_t *argv, uint8_t argc){
    uint8_t tx_packet[20];

    tx_packet[0] = VC0706_RECEIVE;
    tx_packet[1] = SerialNumber;
    tx_packet[2] = cmd;
    tx_packet[3] = argc;

    uint8_t i;
    for(i = 4; i < argc + 4; i++){
        tx_packet[i] = argv[i - 4];
    }

    UART5_WriteBytes(tx_packet, argc + 4);
}

static uint32_t PTC08_ReadResponse(uint8_t *data, uint32_t size, uint32_t timer){
    uint8_t available = 0;
    uint32_t bytesRead = 0;
    TIMER0_SetTimer(timer);

    while(!TIMER0_CheckTimeout()){
        available = UART5_Available();
        if(available > 0){
            TIMER0_DisableTimer();
            bytesRead = UART5_ReadBytes(data, size);
            return bytesRead;
        }
    }

    TIMER0_DisableTimer();
    return bytesRead;
}

static bool PTC08_CheckResponse(uint8_t SerialNumber, uint8_t cmd, uint8_t *data){
    if((data[0] != VC0706_RETURN) || (data[1] != SerialNumber) ||
       (data[2] != cmd)           || (data[3] != 0x00)       ){
        return false;
    }

    return true;
}


bool PTC08_ReadFBuff(uint8_t SerialNumber, uint8_t fBufType, uint8_t controlMode, uint16_t startingAddress, uint8_t dataLenght, uint16_t delay){
    uint8_t argv[] = {  fBufType, controlMode,
                        0, 0, HIGH_BYTE(startingAddress), LOW_BYTE(startingAddress),
                        0, 0, 0, dataLenght,
                        HIGH_BYTE(delay), LOW_BYTE(delay)
                    };

    uint8_t data[20];

    PTC08_SendCommand(READ_FBUF, SerialNumber, argv, sizeof(argv));

    if(PTC08_ReadResponse(data, dataLenght, 40000000) == 0){
        return false;
    }

    if(!PTC08_CheckResponse(SerialNumber, READ_FBUF, data)){
        return false;
    }

   return true;
}

bool PTC08_GetFBuffLen(uint8_t SerialNumber, uint8_t fBuffType, uint32_t *lenght){
    uint8_t argv[] = {  fBuffType };
    uint8_t data[9];

    PTC08_SendCommand(GET_FBUF_LEN, SerialNumber, argv, sizeof(argv));

    if(PTC08_ReadResponse(data, 9, 40000000) == 0){
        return false;
    }

    if(!PTC08_CheckResponse(SerialNumber, GET_FBUF_LEN, data)){
        return false;
    }

    *lenght = (data[5] << 24) | (data[6] << 16) | (data[7] << 8) | data[8];

    return true;
}

bool PTC08_FBuffCtrl(uint8_t SerialNumber, uint8_t controlFlag){
    uint8_t argv[] = { controlFlag };
    uint8_t data[5];

    PTC08_SendCommand(FBUF_CTRL, SerialNumber, argv, sizeof(argv));

    if(PTC08_ReadResponse(data, 9, 40000000) == 0){
        return false;
    }

    if(!PTC08_CheckResponse(SerialNumber, FBUF_CTRL, data)){
        return false;
    }

    return true;

}




