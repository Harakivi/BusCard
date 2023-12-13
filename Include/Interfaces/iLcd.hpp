#pragma once
#include <stdint.h>

namespace Drivers
{
    class iLcd
    {
    public:
        struct FontType
        {
            uint8_t width;
            uint8_t height;
            const uint16_t *data;
        };

        static const FontType Font_7x10;
        static const FontType Font_11x18;
        static const FontType Font_16x26;

        enum Orientation
        {
            PORTRAIT = 0,
            PORTRAIT_MIRROR,
            LANDSCAPE,
            LANDSCAPE_MIRROR,
        };

        enum RGB565Colors
        {
            White565 = 0xFFFF,
            Black565 = 0x0000,
            Blue565 = 0x001F,
            Red565 = 0xF800,
            Magenta565 = 0xF81F,
            Green565 = 0x07E0,
            Cyan565 = 0x7FFF,
            Yellow565 = 0xFFE0,
            Gray565 = 0X8430,
            BRed565 = 0XF81F,
            GRed565 = 0XFFE0,
            GBlue565 = 0X07FF,
            Brown565 = 0XBC40,
            BRRed565 = 0XFC07,
            DarkBlue565 = 0X01CF,
            LightBlue565 = 0X7D7C,
            GrayBlue565 = 0X5458,
            LightGreen565 = 0X841F,
            LGray565 = 0XC618,
            LGrayBlue565 = 0XA651,
            LBBlue565 = 0X2B12,
        };
        virtual uint16_t Width() = 0;
        virtual uint16_t Height() = 0;
        virtual uint16_t GetColorFromRGB(uint8_t R, uint8_t G, uint8_t B) = 0;
        virtual void SetOrientation(Orientation orientation) = 0;
        virtual void FillColor(uint16_t color) = 0;
        virtual void WriteChar(char ch, uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor) = 0;
        virtual void WriteString(const char *str, uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor) = 0;
        virtual void SetFont(const FontType &font) = 0;
    };
}