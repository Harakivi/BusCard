#pragma once

#ifdef __cplusplus
 extern "C" {
#endif

#include "usbd_cdc.h"
#include "stdbool.h"

extern USBD_CDC_ItfTypeDef USBD_CDC_Interface_fops_FS;

bool CDC_Open(void (*onByteReceived)(uint8_t));

uint32_t CDC_GetReceived(uint8_t* Buf, uint32_t Len);

uint8_t CDC_Write(uint8_t* Buf, uint16_t Len);

#ifdef __cplusplus
}
#endif

