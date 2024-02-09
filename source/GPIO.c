#include "GPIO.h"
#include "stm32f4xx_hal.h"

static inline __attribute__((always_inline)) uint32_t GetGPIOMode(uint8_t mode)
{
    switch (mode)
    {
    case 0:
        return GPIO_MODE_INPUT;
        break;
    case 1:
        return GPIO_MODE_OUTPUT_PP;
        break;
    case 2:
        return GPIO_MODE_OUTPUT_OD;
        break;
    default:
        break;
    }
    return 0;
}

__attribute__((always_inline))  void GPIOA_Init(uint8_t pin, uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = 1 << pin;
    GPIO_InitStruct.Mode = GetGPIOMode(mode);
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
__attribute__((always_inline))  void GPIOB_Init(uint8_t pin, uint8_t mode)
{
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = 1 << pin;
    GPIO_InitStruct.Mode = GetGPIOMode(mode);
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
__attribute__((always_inline))  void GPIOC_Init(uint8_t pin, uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = 1 << pin;
    GPIO_InitStruct.Mode = GetGPIOMode(mode);
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
__attribute__((always_inline))  void GPIOD_Init(uint8_t pin, uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOD_CLK_ENABLE();
    GPIO_InitStruct.Pin = 1 << pin;
    GPIO_InitStruct.Mode = GetGPIOMode(mode);
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
__attribute__((always_inline))  void GPIOE_Init(uint8_t pin, uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOE_CLK_ENABLE();
    GPIO_InitStruct.Pin = 1 << pin;
    GPIO_InitStruct.Mode = GetGPIOMode(mode);
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}
__attribute__((always_inline))  void GPIOH_Init(uint8_t pin, uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOH_CLK_ENABLE();
    GPIO_InitStruct.Pin = 1 << pin;
    GPIO_InitStruct.Mode = GetGPIOMode(mode);
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
}

__attribute__((always_inline)) bool GPIOA_Get(uint8_t pin)
{
    return GPIOA->IDR & (1 << pin);
}
__attribute__((always_inline)) bool GPIOB_Get(uint8_t pin)
{
    return GPIOB->IDR & (1 << pin);
}
__attribute__((always_inline)) bool GPIOC_Get(uint8_t pin)
{
    return GPIOC->IDR & (1 << pin);
}
__attribute__((always_inline)) bool GPIOD_Get(uint8_t pin)
{
    return GPIOD->IDR & (1 << pin);
}
__attribute__((always_inline)) bool GPIOE_Get(uint8_t pin)
{
    return GPIOE->IDR & (1 << pin);
}
__attribute__((always_inline)) bool GPIOH_Get(uint8_t pin)
{
    return GPIOH->IDR & (1 << pin);
}

__attribute__((always_inline))  void GPIOA_Set(uint8_t pin, bool val)
{
    GPIOA->ODR &= ~(1 << pin);
    GPIOA->ODR |= (val << pin);
}
__attribute__((always_inline))  void GPIOB_Set(uint8_t pin, bool val)
{
    GPIOB->ODR &= ~(1 << pin);
    GPIOB->ODR |= (val << pin);
}
__attribute__((always_inline))  void GPIOC_Set(uint8_t pin, bool val)
{
    GPIOC->ODR &= ~(1 << pin);
    GPIOC->ODR |= (val << pin);
}
__attribute__((always_inline))  void GPIOD_Set(uint8_t pin, bool val)
{
    GPIOD->ODR &= ~(1 << pin);
    GPIOD->ODR |= (val << pin);
}
__attribute__((always_inline))  void GPIOE_Set(uint8_t pin, bool val)
{
    GPIOE->ODR &= ~(1 << pin);
    GPIOE->ODR |= (val << pin);
}
__attribute__((always_inline))  void GPIOH_Set(uint8_t pin, bool val)
{
    GPIOH->ODR &= ~(1 << pin);
    GPIOH->ODR |= (val << pin);
}
__attribute__((always_inline))  void GPIOA_SetPull(uint8_t pin, uint8_t pull)
{
    GPIOA->PUPDR &= ~(1 << (pin * 2));
    GPIOA->PUPDR |= (pull << (pin * 2));
}
__attribute__((always_inline))  void GPIOB_SetPull(uint8_t pin, uint8_t pull)
{
    GPIOB->PUPDR &= ~(1 << (pin * 2));
    GPIOB->PUPDR |= (pull << (pin * 2));
}
__attribute__((always_inline))  void GPIOC_SetPull(uint8_t pin, uint8_t pull)
{
    GPIOC->PUPDR &= ~(1 << (pin * 2));
    GPIOC->PUPDR |= (pull << (pin * 2));
}
__attribute__((always_inline))  void GPIOD_SetPull(uint8_t pin, uint8_t pull)
{
    GPIOD->PUPDR &= ~(1 << (pin * 2));
    GPIOD->PUPDR |= (pull << (pin * 2));
}
__attribute__((always_inline))  void GPIOE_SetPull(uint8_t pin, uint8_t pull)
{
    GPIOE->PUPDR &= ~(1 << (pin * 2));
    GPIOE->PUPDR |= (pull << (pin * 2));
}
__attribute__((always_inline))  void GPIOH_SetPull(uint8_t pin, uint8_t pull)
{
    GPIOH->PUPDR &= ~(1 << (pin * 2));
    GPIOH->PUPDR |= (pull << (pin * 2));
}