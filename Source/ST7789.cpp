#include "ST7789.hpp"

using namespace Drivers;

ST7789::ST7789(iSpi &spi, iGpio &dcPin, iGpio &resPin, iGpio &csPin) : _spi(spi), _dcPin(dcPin), _resPin(resPin), _csPin(csPin), _endOfTransfer(true) {}

bool ST7789::Init()
{
    bool res = true;
    res &= _spi.Init(*this);
    res &= _dcPin.Init(iGpio::OutputPushPull);
    res &= _resPin.Init(iGpio::OutputPushPull);
    res &= _csPin.Init(iGpio::OutputPushPull);
    _csPin.SetVal(true);
    return res;
}

void ST7789::onWriteDone()
{
    if (_endOfTransfer)
    {
        _csPin.SetVal(true);
    }
}

void ST7789::WriteCommand(uint8_t &cmd)
{
    while (_spi.IsBusy())
    {
    }
    _csPin.SetVal(false);
    _dcPin.SetVal(false);
    _spi.Write(&cmd, sizeof(cmd));
}

void ST7789::WriteData(uint8_t *buff, uint32_t buffSize)
{
    while (_spi.IsBusy())
    {
    }
    _csPin.SetVal(false);
    _dcPin.SetVal(true);
    while (buffSize > 0)
    {
        while (_spi.IsBusy())
        {
        }
        uint16_t chunk = buffSize > 65535 ? 65535 : buffSize;
        buffSize -= chunk;
        _endOfTransfer = buffSize > 0 ? false : true;
        _spi.Write(buff, chunk);
        buff += chunk;
    }
}