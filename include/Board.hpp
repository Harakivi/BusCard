#pragma once
#include "Spi.hpp"
#include "UsbCdc.hpp"
#include "GPIO.hpp"
#include "iSpi.hpp"
#include "iGpio.hpp"
#include "iDelayer.hpp"
#include "iUart.hpp"
#include "Drivers/ST7789.hpp"
#include "Utils/GFXLib.hpp"

namespace Hardware
{
    class BusCard
    {
    private:
        BusCard() {}
        static bool _initied;
        static bool RCC_Init();
        typedef Spi<1, true> LcdSpi;
        typedef Gpio<GpioA, 2> LcdDcPin;
        typedef Gpio<GpioA, 3> LcdResPin;
        typedef Gpio<GpioA, 4> LcdCsPin;
        typedef Gpio<GpioC, 13> LedPin;
        typedef UsbCdc CliUartType;
        typedef typename Drivers::ST7789<BusCard::LcdSpi, BusCard::LcdDcPin, BusCard::LcdResPin, BusCard::LcdCsPin> LcdType;

    public:
        static iUart &cliUart;
        static Utils::GFX &Lcd;
        static void Init();
    };
}
