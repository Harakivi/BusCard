#pragma once

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "usbd_def.h"

bool MX_USB_DEVICE_Init(void);


#ifdef __cplusplus
}
#endif
