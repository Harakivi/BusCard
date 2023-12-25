#pragma once
#include "stdint.h"

namespace Hardware
{
    class SpiHandler
    {
    public:
        virtual void onWriteDone() = 0;
    };

    class iSpi
    {
    public:
        virtual bool Init(SpiHandler &handler) = 0;
        virtual void Write(uint8_t *buff, uint32_t size) = 0;
        virtual volatile bool IsBusy() = 0;
    };
}