#include "PTC08.h"
#include "utils/uartstdio.h"

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
        if(available >= size){
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


static uint8_t *PTC08_RunCommand(uint8_t SerialNumber, uint8_t cmd, uint8_t *argv, uint8_t argc, uint8_t ResponseLenght){
    PTC08_SendCommand(cmd, SerialNumber, argv, argc);

    static uint8_t Response[20];

    if(!PTC08_ReadResponse(Response, ResponseLenght, PTC08_RESPONSE_TIMEOUT)){
        return NULL;
    }

    if(!PTC08_CheckResponse(SerialNumber, cmd, Response)){
        return NULL;
    }

    return Response;
}


bool PTC08_ReadFBuff(uint8_t SerialNumber, uint8_t fBufType, uint8_t ControlMode, uint16_t StartingAddress, uint16_t DataLenght, uint16_t Delay, uint8_t *data){
    uint8_t argv[] = {  fBufType, ControlMode,
                        0, 0, HIGH_BYTE(StartingAddress), LOW_BYTE(StartingAddress),
                        0, 0, HIGH_BYTE(DataLenght), LOW_BYTE(DataLenght),
                        HIGH_BYTE(Delay), LOW_BYTE(Delay)
                    };

    uint8_t *Response = PTC08_RunCommand(SerialNumber, READ_FBUF, argv, sizeof(argv), 5);
    if(Response == NULL){
        return false;
    }

    if(!PTC08_ReadResponse(data, DataLenght + 5, PTC08_RESPONSE_TIMEOUT)){
        return false;
    }

   return true;
}

uint32_t PTC08_GetFBuffLen(uint8_t SerialNumber, uint8_t fBuffType){
    uint8_t argv[] = {  fBuffType };

    uint8_t *Response = PTC08_RunCommand(SerialNumber, GET_FBUF_LEN, argv, sizeof(argv), 9);
    if(Response == NULL){
        return 0;
    }

    uint32_t lenght = (Response[5] << 24) | (Response[6] << 16) | (Response[7] << 8) | Response[8];

    return lenght;
}

bool PTC08_FBuffCtrl(uint8_t SerialNumber, uint8_t controlFlag){
    uint8_t argv[] = { controlFlag };

    uint8_t *Response = PTC08_RunCommand(SerialNumber, FBUF_CTRL, argv, sizeof(argv), 5);
    if(Response == NULL){
        return false;
    }

    return true;
}

uint8_t *PTC08_Reset(uint8_t SerialNumber){
    uint8_t *Response = PTC08_RunCommand(SerialNumber, SYSTEM_RESET, NULL, 0, 5);
    if(Response == NULL){
        return false;
    }

    static uint8_t info[49];
    if(!PTC08_ReadResponse(info, 49, PTC08_RESPONSE_TIMEOUT)){
        return NULL;
    }

    return info;
}

bool PTC08_ReadData(uint8_t SerialNumber, uint8_t DeviceType, uint8_t DataNumToRead, uint32_t ConfigInfo, uint8_t *data){
    uint8_t argv[6];
    uint8_t *Response;

    argv[1] = DataNumToRead;

    switch(DeviceType){
    case CHIP_REGISTER:
        argv[0] = CHIP_REGISTER;
        argv[2] = FOURTH_BYTE(ConfigInfo);
        argv[3] = THIRD_BYTE(ConfigInfo);
        Response = PTC08_RunCommand(SerialNumber, READ_DATA, argv, 4, 5);
        break;
    case SENSOR_REGISTER:
        argv[0] = SENSOR_REGISTER;
        argv[2] = FOURTH_BYTE(ConfigInfo);
        argv[3] = THIRD_BYTE(ConfigInfo);
        argv[4] = SECOND_BYTE(ConfigInfo);
        Response = PTC08_RunCommand(SerialNumber, READ_DATA, argv, 5, 5);
        break;
    case CCIR656_REGISTER:
        argv[0] = CCIR656_REGISTER;
        argv[2] = FOURTH_BYTE(ConfigInfo);
        argv[3] = THIRD_BYTE(ConfigInfo);
        argv[4] = SECOND_BYTE(ConfigInfo);
        Response = PTC08_RunCommand(SerialNumber, READ_DATA, argv, 5, 5);
        break;
    case I2C_EEPROM:
        argv[0] = I2C_EEPROM;
        argv[2] = FOURTH_BYTE(ConfigInfo);
        argv[3] = THIRD_BYTE(ConfigInfo);
        Response = PTC08_RunCommand(SerialNumber, READ_DATA, argv, 4, 5);
        break;
    case SPI_EEPROM:
        argv[0] = SPI_EEPROM;
        argv[2] = FOURTH_BYTE(ConfigInfo);
        argv[3] = THIRD_BYTE(ConfigInfo);
        Response = PTC08_RunCommand(SerialNumber, READ_DATA, argv, 4, 5);
        break;
    case SPI_FLASH:
        argv[0] = SPI_FLASH;
        argv[2] = FOURTH_BYTE(ConfigInfo);
        argv[3] = THIRD_BYTE(ConfigInfo);
        argv[4] = SECOND_BYTE(ConfigInfo);
        argv[5] = FIRST_BYTE(ConfigInfo);
        Response = PTC08_RunCommand(SerialNumber, READ_DATA, argv, 6, 6);
        break;
    }

    if(Response == NULL){
        return false;;
    }

    if(!PTC08_ReadResponse(data, DataNumToRead, PTC08_RESPONSE_TIMEOUT)){
        return false;
    }

    return true;
}




