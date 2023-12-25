#pragma once
#include <stdint.h>

namespace Utils
{
    class iDelayer
    {
    public:
        virtual void Delay(uint32_t delay) = 0;
    };
}