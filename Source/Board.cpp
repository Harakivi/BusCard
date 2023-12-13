#include "Board.hpp"
#include "stm32f4xx_hal.h"
#include "usb_device.h"
#include "Interfaces/iDelayer.hpp"

using namespace Hardware;

class HalDelayer : public Utils::iDelayer
{
public:
  virtual void Delay(uint32_t delay)
  {
    HAL_Delay(delay);
  }
};

bool BusCard::_initied = false;
static HalDelayer halDelayer;

iUart &BusCard::cliUart = BusCard::CliUartType::Get();
Drivers::iLcd &BusCard::Lcd = BusCard::LcdType::Get();

bool BusCard::RCC_Init()
{
  bool res = true;
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  res &= HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK;
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  res &= HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) == HAL_OK;

  return res;
}

void BusCard::Init()
{
  _initied = true;
  _initied &= RCC_Init();
  SystemCoreClockUpdate();
  _initied &= HAL_Init() == HAL_OK;
  _initied &= MX_USB_DEVICE_Init();
  LcdType::Get().Init(240, 240, halDelayer, Drivers::iLcd::LANDSCAPE_MIRROR);
}

extern "C" void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
}