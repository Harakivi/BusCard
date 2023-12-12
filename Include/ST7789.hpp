#pragma once
#include "Interfaces.hpp"
#include <stdint.h>

namespace Drivers
{
    using namespace Hardware;
    class ST7789: public Hardware::SpiHandler
    {
    private:
        iSpi &_spi;
        iGpio &_dcPin;
        iGpio &_resPin;
        iGpio &_csPin;
        bool _endOfTransfer;
        void WriteCommand(uint8_t &cmd);
        void WriteData(uint8_t *buff, uint32_t buff_size);
        virtual void onWriteDone();
    public:
        ST7789(iSpi &spi, iGpio &dcPin, iGpio &resPin, iGpio &csPin);

        bool Init();
    };
}