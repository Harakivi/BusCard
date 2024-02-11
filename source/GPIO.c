#include "GPIO.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

static inline __attribute__((always_inline)) uint32_t GetGPIOMode(uint8_t mode, uint8_t edge)
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
    case 3:
        switch (edge)
        {
        case 1:
            return GPIO_MODE_IT_RISING;
            break;
        case 2:
            return GPIO_MODE_IT_FALLING;
            break;
        case 3:
            return GPIO_MODE_IT_RISING_FALLING;
            break;

        default:
            return GPIO_MODE_INPUT;
            break;
        }
    default:
        break;
    }
    return 0;
}

__attribute__((always_inline)) void GPIOA_Init(uint8_t pin, uint8_t mode, uint8_t edge)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = 1 << pin;
    GPIO_InitStruct.Mode = GetGPIOMode(mode, edge);
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
__attribute__((always_inline)) void GPIOB_Init(uint8_t pin, uint8_t mode, uint8_t edge)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = 1 << pin;
    GPIO_InitStruct.Mode = GetGPIOMode(mode, edge);
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
__attribute__((always_inline)) void GPIOC_Init(uint8_t pin, uint8_t mode, uint8_t edge)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = 1 << pin;
    GPIO_InitStruct.Mode = GetGPIOMode(mode, edge);
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
__attribute__((always_inline)) void GPIOD_Init(uint8_t pin, uint8_t mode, uint8_t edge)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOD_CLK_ENABLE();
    GPIO_InitStruct.Pin = 1 << pin;
    GPIO_InitStruct.Mode = GetGPIOMode(mode, edge);
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
__attribute__((always_inline)) void GPIOE_Init(uint8_t pin, uint8_t mode, uint8_t edge)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOE_CLK_ENABLE();
    GPIO_InitStruct.Pin = 1 << pin;
    GPIO_InitStruct.Mode = GetGPIOMode(mode, edge);
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}
__attribute__((always_inline)) void GPIOH_Init(uint8_t pin, uint8_t mode, uint8_t edge)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOH_CLK_ENABLE();
    GPIO_InitStruct.Pin = 1 << pin;
    GPIO_InitStruct.Mode = GetGPIOMode(mode, edge);
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

__attribute__((always_inline)) void GPIOA_Set(uint8_t pin, bool val)
{
    GPIOA->ODR &= ~(1 << pin);
    GPIOA->ODR |= (val << pin);
}
__attribute__((always_inline)) void GPIOB_Set(uint8_t pin, bool val)
{
    GPIOB->ODR &= ~(1 << pin);
    GPIOB->ODR |= (val << pin);
}
__attribute__((always_inline)) void GPIOC_Set(uint8_t pin, bool val)
{
    GPIOC->ODR &= ~(1 << pin);
    GPIOC->ODR |= (val << pin);
}
__attribute__((always_inline)) void GPIOD_Set(uint8_t pin, bool val)
{
    GPIOD->ODR &= ~(1 << pin);
    GPIOD->ODR |= (val << pin);
}
__attribute__((always_inline)) void GPIOE_Set(uint8_t pin, bool val)
{
    GPIOE->ODR &= ~(1 << pin);
    GPIOE->ODR |= (val << pin);
}
__attribute__((always_inline)) void GPIOH_Set(uint8_t pin, bool val)
{
    GPIOH->ODR &= ~(1 << pin);
    GPIOH->ODR |= (val << pin);
}
__attribute__((always_inline)) void GPIOA_SetPull(uint8_t pin, uint8_t pull)
{
    GPIOA->PUPDR &= ~(1 << (pin * 2));
    GPIOA->PUPDR |= (pull << (pin * 2));
}
__attribute__((always_inline)) void GPIOB_SetPull(uint8_t pin, uint8_t pull)
{
    GPIOB->PUPDR &= ~(1 << (pin * 2));
    GPIOB->PUPDR |= (pull << (pin * 2));
}
__attribute__((always_inline)) void GPIOC_SetPull(uint8_t pin, uint8_t pull)
{
    GPIOC->PUPDR &= ~(1 << (pin * 2));
    GPIOC->PUPDR |= (pull << (pin * 2));
}
__attribute__((always_inline)) void GPIOD_SetPull(uint8_t pin, uint8_t pull)
{
    GPIOD->PUPDR &= ~(1 << (pin * 2));
    GPIOD->PUPDR |= (pull << (pin * 2));
}
__attribute__((always_inline)) void GPIOE_SetPull(uint8_t pin, uint8_t pull)
{
    GPIOE->PUPDR &= ~(1 << (pin * 2));
    GPIOE->PUPDR |= (pull << (pin * 2));
}
__attribute__((always_inline)) void GPIOH_SetPull(uint8_t pin, uint8_t pull)
{
    GPIOH->PUPDR &= ~(1 << (pin * 2));
    GPIOH->PUPDR |= (pull << (pin * 2));
}

static void (*_gpioHandler[15])();

__attribute__((always_inline)) void GPIO_SetIrqHandler(void (*gpioHandler)(), uint16_t pin)
{
    if (pin < 16)
    {
        _gpioHandler[pin] = gpioHandler;

        if (pin == 0)
        {
            HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI0_IRQn);
        }
        else if (pin == 1)
        {
            HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI1_IRQn);
        }
        else if (pin == 2)
        {
            HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI2_IRQn);
        }
        else if (pin == 3)
        {
            HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI3_IRQn);
        }
        else if (pin == 4)
        {
            HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI4_IRQn);
        }
        else if (pin >= 5 && pin <= 9)
        {
            HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
        }
        else if (pin >= 10 && pin <= 15)
        {

            HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
        }
    }
}

void EXTI_IRQ(uint16_t GPIO_Pin)
{
    if (__HAL_GPIO_EXTI_GET_IT(1 << GPIO_Pin) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(1 << GPIO_Pin);
        _gpioHandler[GPIO_Pin]();
    }
}

void EXTI0_IRQHandler(void)
{
    EXTI_IRQ(0);
}
void EXTI1_IRQHandler(void)
{
    EXTI_IRQ(1);
}
void EXTI2_IRQHandler(void)
{
    EXTI_IRQ(2);
}
void EXTI3_IRQHandler(void)
{
    EXTI_IRQ(3);
}
void EXTI4_IRQHandler(void)
{
    EXTI_IRQ(4);
}
void EXTI9_5_IRQHandler(void)
{
    EXTI_IRQ(5);
    EXTI_IRQ(6);
    EXTI_IRQ(7);
    EXTI_IRQ(8);
    EXTI_IRQ(9);
}
void EXTI15_10_IRQHandler(void)
{
    EXTI_IRQ(10);
    EXTI_IRQ(11);
    EXTI_IRQ(12);
    EXTI_IRQ(13);
    EXTI_IRQ(14);
    EXTI_IRQ(15);
}
