#include "stm32f4xx_hal.h"
#include "FreeRTOSConfig.h"
#include "TaskManager.hpp"

extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

extern "C" void xPortSysTickHandler( void );

extern "C" void SysTick_Handler(void)
{
  if(TaskManager::Get()->IsStarted())
    xPortSysTickHandler();

  HAL_IncTick();
}

extern "C" void OTG_FS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}