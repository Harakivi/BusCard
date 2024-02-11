#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "usbd_storage_if.h"
#include "stdint.h"
#include "usbd_composite_builder.h"

USBD_HandleTypeDef hUsbDeviceFS;

void Error_Handler();

uint8_t MSC_EpAdd[2] = {MSC_EPIN_ADDR, MSC_EPOUT_ADDR};     /* MSC Endpoint Adress */
uint8_t CDC_EpAdd[3] = {CDC_IN_EP, CDC_OUT_EP, CDC_CMD_EP}; /* CDC Endpoint Adress First Instance */

bool MX_USB_DEVICE_Init(void)
{
  bool res = true;

  res &= USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) == USBD_OK;

#if USBD_CMPSIT_ACTIVATE_MSC == 1
  res &= USBD_RegisterClassComposite(&hUsbDeviceFS, &USBD_MSC, CLASS_TYPE_MSC, MSC_EpAdd) == USBD_OK;
#endif

// #if USBD_CMPSIT_ACTIVATE_CDC == 1
//   res &= USBD_RegisterClassComposite(&hUsbDeviceFS, &USBD_CDC, CLASS_TYPE_CDC, CDC_EpAdd) == USBD_OK;
// #endif

#if USBD_CMPSIT_ACTIVATE_MSC == 1
  if (USBD_CMPSIT_SetClassID(&hUsbDeviceFS, CLASS_TYPE_MSC, 0) != 0xFF)
  {
    res &= USBD_MSC_RegisterStorage(&hUsbDeviceFS, &USBD_Storage_Interface_fops_FS) == USBD_OK;
  }
#endif

// #if USBD_CMPSIT_ACTIVATE_CDC == 1
//   if (USBD_CMPSIT_SetClassID(&hUsbDeviceFS, CLASS_TYPE_CDC, 0) != 0xFF)
//   {
//     res &= USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_CDC_Interface_fops_FS) == USBD_OK;
//   }
// #endif

  res &= USBD_Start(&hUsbDeviceFS) == USBD_OK;

  return res;
}