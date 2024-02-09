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
        typedef Gpio<GpioB, 14> LcdDcPin;
        typedef Gpio<GpioB, 15> LcdResPin;
        typedef Gpio<GpioC, 6> LcdCsPin;

        typedef UsbCdc CliUartType;
        typedef typename Drivers::ST7789<BusCard::LcdSpi, BusCard::LcdDcPin, BusCard::LcdResPin, BusCard::LcdCsPin> LcdType;

    public:
        static iUart &cliUart;
        static Utils::GFX &Lcd;
        typedef Gpio<GpioB, 13> BtnRight;
        typedef Gpio<GpioB, 12> BtnLeft;
        typedef Gpio<GpioA, 0> BtnStart;
        typedef Gpio<GpioA, 1> BtnSelect;
        typedef Gpio<GpioA, 2> BtnDown;
        typedef Gpio<GpioA, 3> BtnUp;
        typedef Gpio<GpioB, 9> BtnA;
        typedef Gpio<GpioB, 8> BtnB;
        static void Init();
    };
}
