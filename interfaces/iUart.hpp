#pragma once
#include "stdint.h"

namespace Hardware
{
    class UartHandler
    {
    public:
        virtual void onByteReceived(uint8_t data) = 0;
    };

    class iUart
    {
    public:
        virtual bool Open(uint32_t BaudRate, void (*onByteReceived)(uint8_t) = nullptr) = 0;

        virtual bool Open(uint32_t BaudRate, UartHandler *handler) = 0;

        virtual bool Write(uint8_t *buffer, uint32_t count) = 0;

        virtual uint32_t GetReceived(uint8_t *buffer, uint32_t size) = 0;
    };
}