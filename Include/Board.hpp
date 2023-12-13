#pragma once
#include "Spi.hpp"
#include "UsbCdc.hpp"
#include "GPIO.hpp"
#include "Interfaces/iSpi.hpp"
#include "Interfaces/iGpio.hpp"
#include "Interfaces/iDelayer.hpp"
#include "Interfaces/iUart.hpp"
#include "Drivers/ST7789.hpp"

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
        static Drivers::iLcd &Lcd;
        static void Init();
    };
}
