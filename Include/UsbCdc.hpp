#pragma once
#include "Interfaces.hpp"
#include "usb_dev/usbd_cdc_if.h"
#include <stdint.h>


namespace Hardware
{
    class UsbCdc : public iUart
    {
    private:
        bool _opened;
        UartHandler *_handler;
        UsbCdc() : _opened(true), _handler(nullptr) {}

        static void _onByteReceived(uint8_t data)
        {
            if (UsbCdc::Get()._handler != nullptr)
                UsbCdc::Get()._handler->onByteReceived(data);
        }

    public:
        static UsbCdc& Get()
        {
            static UsbCdc instance = UsbCdc();
            return instance;
        }

        bool Open(uint32_t BaudRate, void (*onByteReceived)(uint8_t) = nullptr)
        {
            return _opened;
        }

        bool Open(uint32_t BaudRate, UartHandler *handler)
        {
            _handler = handler;
            return CDC_Open(_onByteReceived);
        }
        bool Write(uint8_t *buffer, uint32_t count)
        {
            if (_opened)
            {
                while(!CDC_Write(buffer, count))
                {}
                return true;
            }
            return false;
        }

        uint32_t GetReceived(uint8_t *buffer, uint32_t size)
        {
            if (_opened)
            {
                return CDC_GetReceived(buffer, size);
            }
            return 0;
        }
    };
}