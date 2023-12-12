#pragma once
#include "stdbool.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

bool Spi1Init(void (*onTransferComplete)());
bool Spi1DmaInit(void (*onTransferComplete)());
void Spi1Write(uint8_t* buff, uint16_t size);
void Spi1WriteDma(uint8_t* buff, uint16_t size);

#ifdef __cplusplus
}
#endif