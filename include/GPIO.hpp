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
        GpioHandler *_handler;

        static void _gpioHandler()
        {
            Gpio *_this = &Gpio::Get();
            if (_this->_handler != nullptr)
                _this->_handler->Handler(_this->GetVal());
        }

        Gpio() : _handler(nullptr){};

    public:
        static Gpio &Get()
        {
            static Gpio instance;
            return instance;
        }

        virtual bool Init(iGpio::GPIO_Modes mode, GpioHandler *handler = nullptr, iGpio::Interrupt_Modes edge = None)
        {
            bool res = false;
            if constexpr (Port == GpioA && Pin < 16)
            {
                GPIOA_Init(Pin, mode, edge);
                res = true;
            }
            if constexpr (Port == GpioB && Pin < 16)
            {
                GPIOB_Init(Pin, mode, edge);
                res = true;
            }
            if constexpr (Port == GpioC && Pin < 16)
            {
                GPIOC_Init(Pin, mode, edge);
                res = true;
            }
            if constexpr (Port == GpioD && Pin < 16)
            {
                GPIOD_Init(Pin, mode, edge);
                res = true;
            }
            if constexpr (Port == GpioE && Pin < 16)
            {
                GPIOE_Init(Pin, mode, edge);
                res = true;
            }
            if constexpr (Port == GpioH && Pin < 16)
            {
                GPIOH_Init(Pin, mode, edge);
                res = true;
            }
            _handler = handler;
            if (_handler)
            {
                GPIO_SetIrqHandler(_gpioHandler, Pin);
            }
            return res;
        }

        virtual inline bool GetVal()
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
        virtual inline void SetVal(bool val)
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