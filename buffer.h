#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    uint8_t *Buffer;
    volatile uint32_t Buffer_WriteIndex;
    volatile uint32_t Buffer_ReadIndex;
    uint32_t Buffer_Size;
}Buffer;

void Buffer_Init(Buffer *b, uint8_t *buffer, uint32_t buffer_size);
bool Buffer_Write(Buffer *b, uint8_t *data, uint32_t size, uint32_t *bytesWritten);
bool Buffer_Read(Buffer *b, uint8_t *data, uint32_t size, uint32_t *bytesRead);
bool Buffer_Full(Buffer *b);
bool Buffer_Empty(Buffer *b);
uint32_t Buffer_Available(Buffer *b);

#endif /* BUFFER_H_ */
