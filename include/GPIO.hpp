#pragma once
#include "iGpio.hpp"
#include "GPIO.h"

namespace Hardware
{
    enum GPIO_Ports
    {
        GpioA = 0,
        GpioB,
        GpioC,
        GpioD,
        GpioE,
        GpioH,
    };

    template <GPIO_Ports Port, uint8_t Pin>
    class Gpio : public iGpio
    {
    private:
        Gpio(){};

    public:
        static Gpio &Get()
        {
            static Gpio instance;
            return instance;
        }
        virtual bool Init(iGpio::GPIO_Modes mode)
        {
            if constexpr (Port == GpioA && Pin < 16)
            {
                GPIOA_Init(Pin, mode);
                return true;
            }
            if constexpr (Port == GpioB && Pin < 16)
            {
                GPIOB_Init(Pin, mode);
                return true;
            }
            if constexpr (Port == GpioC && Pin < 16)
            {
                GPIOC_Init(Pin, mode);
                return true;
            }
            if constexpr (Port == GpioD && Pin < 16)
            {
                GPIOD_Init(Pin, mode);
                return true;
            }
            if constexpr (Port == GpioE && Pin < 16)
            {
                GPIOE_Init(Pin, mode);
                return true;
            }
            if constexpr (Port == GpioH && Pin < 16)
            {
                GPIOH_Init(Pin, mode);
                return true;
            }
            return false;
        }
        virtual bool GetVal()
        {
            if constexpr (Port == GpioA && Pin < 16)
            {
                return GPIOA_Get(Pin);
            }
            if constexpr (Port == GpioB && Pin < 16)
            {
                return GPIOB_Get(Pin);
            }
            if constexpr (Port == GpioC && Pin < 16)
            {
                return GPIOC_Get(Pin);
            }
            if constexpr (Port == GpioD && Pin < 16)
            {
                return GPIOD_Get(Pin);
            }
            if constexpr (Port == GpioE && Pin < 16)
            {
                return GPIOE_Get(Pin);
            }
            if constexpr (Port == GpioH && Pin < 16)
            {
                return GPIOH_Get(Pin);
            }
            return false;
        }
        virtual void SetVal(bool val)
        {
            if constexpr (Port == GpioA && Pin < 16)
            {
                GPIOA_Set(Pin, val);
            }
            if constexpr (Port == GpioB && Pin < 16)
            {
                GPIOB_Set(Pin, val);
            }
            if constexpr (Port == GpioC && Pin < 16)
            {
                GPIOC_Set(Pin, val);
            }
            if constexpr (Port == GpioD && Pin < 16)
            {
                GPIOD_Set(Pin, val);
            }
            if constexpr (Port == GpioE && Pin < 16)
            {
                GPIOE_Set(Pin, val);
            }
            if constexpr (Port == GpioH && Pin < 16)
            {
                GPIOH_Set(Pin, val);
            }
        }
        virtual void SetPull(iGpio::Pull_Modes pull)
        {
            if constexpr (Port == GpioA && Pin < 16)
            {
                GPIOA_SetPull(Pin, pull);
            }
            if constexpr (Port == GpioB && Pin < 16)
            {
                GPIOB_SetPull(Pin, pull);
            }
            if constexpr (Port == GpioC && Pin < 16)
            {
                GPIOC_SetPull(Pin, pull);
            }
            if constexpr (Port == GpioD && Pin < 16)
            {
                GPIOD_SetPull(Pin, pull);
            }
            if constexpr (Port == GpioE && Pin < 16)
            {
                GPIOE_SetPull(Pin, pull);
            }
            if constexpr (Port == GpioH && Pin < 16)
            {
                GPIOH_SetPull(Pin, pull);
            }
        }
    };

}