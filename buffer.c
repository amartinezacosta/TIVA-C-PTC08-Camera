#include "buffer.h"

void Buffer_Init(Buffer *b, uint8_t *buffer, uint32_t buffer_size){
    b->Buffer = buffer;
    b->Buffer_ReadIndex = 0;
    b->Buffer_WriteIndex = 0;
    b->Buffer_Size = buffer_size;
}

bool Buffer_Write(Buffer *b, uint8_t *data, uint32_t size, uint32_t *bytesWritten){
    int32_t i;
    for(i = 0; i < size; i++){
        /*write data only if buffer is not full*/
        if(Buffer_Full(b)){
            *bytesWritten = i;
            return false;
        }
        else{
            b->Buffer[b->Buffer_WriteIndex] = data[i];
            b->Buffer_WriteIndex = (b->Buffer_WriteIndex + 1) % b->Buffer_Size;
        }
    }

    *bytesWritten = i;

    return true;
}

bool Buffer_Read(Buffer *b, uint8_t *data, uint32_t size, uint32_t *bytesRead){
    uint8_t i;

    for(i = 0; i < size; i++){
        /*read data only if buffer is not empty*/
        if(Buffer_Empty(b)){
            *bytesRead = i;
            return false;
        }
        else{
            data[i] = b->Buffer[b->Buffer_ReadIndex];
            b->Buffer_ReadIndex = (b->Buffer_ReadIndex + 1) % b->Buffer_Size;;
        }
    }

    *bytesRead = i;
    return true;
}


uint32_t Buffer_Available(Buffer *b){
    return b->Buffer_WriteIndex - b->Buffer_ReadIndex;
}

bool Buffer_Full(Buffer *b){
    if(b->Buffer_WriteIndex + 1 == b->Buffer_ReadIndex){
        return true;
    }
    else{
        return false;
    }
}

bool Buffer_Empty(Buffer *b){
    if(b->Buffer_ReadIndex == b->Buffer_WriteIndex){
        return true;
    }
    else{
        return false;
    }
}

