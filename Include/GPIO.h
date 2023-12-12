#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void GPIOA_Init(uint8_t pin, uint8_t mode);
    void GPIOB_Init(uint8_t pin, uint8_t mode);
    void GPIOC_Init(uint8_t pin, uint8_t mode);
    void GPIOD_Init(uint8_t pin, uint8_t mode);
    void GPIOE_Init(uint8_t pin, uint8_t mode);
    void GPIOH_Init(uint8_t pin, uint8_t mode);

    bool GPIOA_Get(uint8_t pin);
    bool GPIOB_Get(uint8_t pin);
    bool GPIOC_Get(uint8_t pin);
    bool GPIOD_Get(uint8_t pin);
    bool GPIOE_Get(uint8_t pin);
    bool GPIOH_Get(uint8_t pin);

    void GPIOA_Set(uint8_t pin, bool val);
    void GPIOB_Set(uint8_t pin, bool val);
    void GPIOC_Set(uint8_t pin, bool val);
    void GPIOD_Set(uint8_t pin, bool val);
    void GPIOE_Set(uint8_t pin, bool val);
    void GPIOH_Set(uint8_t pin, bool val);
    
    void GPIOA_SetPull(uint8_t pin, uint8_t pull);
    void GPIOB_SetPull(uint8_t pin, uint8_t pull);
    void GPIOC_SetPull(uint8_t pin, uint8_t pull);
    void GPIOD_SetPull(uint8_t pin, uint8_t pull);
    void GPIOE_SetPull(uint8_t pin, uint8_t pull);
    void GPIOH_SetPull(uint8_t pin, uint8_t pull);

#ifdef __cplusplus
}
#endif