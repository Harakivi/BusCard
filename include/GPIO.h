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

    inline bool GPIOA_Get(uint8_t pin);
    inline bool GPIOB_Get(uint8_t pin);
    inline bool GPIOC_Get(uint8_t pin);
    inline bool GPIOD_Get(uint8_t pin);
    inline bool GPIOE_Get(uint8_t pin);
    inline bool GPIOH_Get(uint8_t pin);

    inline void GPIOA_Set(uint8_t pin, bool val);
    inline void GPIOB_Set(uint8_t pin, bool val);
    inline void GPIOC_Set(uint8_t pin, bool val);
    inline void GPIOD_Set(uint8_t pin, bool val);
    inline void GPIOE_Set(uint8_t pin, bool val);
    inline void GPIOH_Set(uint8_t pin, bool val);
    
    void GPIOA_SetPull(uint8_t pin, uint8_t pull);
    void GPIOB_SetPull(uint8_t pin, uint8_t pull);
    void GPIOC_SetPull(uint8_t pin, uint8_t pull);
    void GPIOD_SetPull(uint8_t pin, uint8_t pull);
    void GPIOE_SetPull(uint8_t pin, uint8_t pull);
    void GPIOH_SetPull(uint8_t pin, uint8_t pull);

#ifdef __cplusplus
}
#endif