#include "Board.hpp"
#include "usbd_cdc_if.h"

int main()
{
  BoardInit();
  
  while(true)
  {
    HAL_Delay(1000);
    CDC_Transmit_FS((uint8_t *)"Start\r\n", 7);
  }
}
