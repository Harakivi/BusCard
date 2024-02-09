#include "usbd_cdc_if.h"
#include "usbd_composite_builder.h"

static uint8_t RxBufferFS[256];
static uint16_t RxLen = 0;
static void (*_onByteReceived)(uint8_t);

extern USBD_HandleTypeDef hUsbDeviceFS;


static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);

USBD_CDC_ItfTypeDef USBD_CDC_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS,
  NULL
};

bool CDC_Open(void (*onByteReceived)(uint8_t))
{
  _onByteReceived = onByteReceived;
  return true;
}

static int8_t CDC_Init_FS(void)
{
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, RxBufferFS);
  return (USBD_OK);
}

static int8_t CDC_DeInit_FS(void)
{
  return (USBD_OK);
}

static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  switch(cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

    case CDC_SET_COMM_FEATURE:

    break;

    case CDC_GET_COMM_FEATURE:

    break;

    case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:

    break;

    case CDC_GET_LINE_CODING:

    break;

    case CDC_SET_CONTROL_LINE_STATE:

    break;

    case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
}

static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
  USBD_CDC_ReceivePacket(&hUsbDeviceFS);

  RxLen += *Len;
  if (_onByteReceived != NULL)
  {
    while (RxLen)
    {
      _onByteReceived(*Buf++);
      RxLen--;
    }
  }
  return (USBD_OK);
}

uint32_t CDC_GetReceived(uint8_t* Buf, uint32_t Len)
{
  uint32_t writted = 0;
  while (writted < Len && writted < RxLen)
  {
    *Buf = *(RxBufferFS + writted);
     writted++;
  }
  return writted;
}

volatile uint8_t CDC_Write(uint8_t* Buf, uint16_t Len)
{
  uint32_t classId = 0U;
#ifdef USE_USBD_COMPOSITE
  classId = USBD_CMPSIT_GetClassID(&hUsbDeviceFS, CLASS_TYPE_CDC, 0);
#endif
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  if (hcdc->TxState != 0){
   return USBD_BUSY;
  }
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len, classId);
  uint8_t res = USBD_CDC_TransmitPacket(&hUsbDeviceFS, classId);
  return res;
}