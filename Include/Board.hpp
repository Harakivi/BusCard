#pragma once
#include "Spi.hpp"
#include "UsbCdc.hpp"
#include "GPIO.hpp"
#include "ST7789.hpp"

namespace Hardware
{
    class BusCard
    {

    private:
        static bool _initied;

        static bool RCC_Init();

        BusCard() {}

        typedef Spi<1, true> DisplaySpi;
        typedef Gpio<GpioA, 2> Lcd_DC_Pin;
        typedef Gpio<GpioA, 3> Lcd_RES_Pin;
        typedef Gpio<GpioA, 4> Lcd_CS_Pin;
        typedef Gpio<GpioC, 13> Led_Pin;

    public:
        typedef UsbCdc cliUart;

        static Drivers::ST7789 lcd;

        static void Init();
    };
}
