#pragma once
#include <stdint.h>
#include "iSpi.hpp"
#include "iGpio.hpp"
#include "iDelayer.hpp"
#include "iLcd.hpp"
#include "Utils/GFXLib.hpp"

namespace Drivers
{
    using namespace Hardware;
    using namespace Utils;
    template <class LcdSpiType, class DcPinType, class ResPinType, class CsPinType>
    class ST7789 : public GFX, public Hardware::SpiHandler
    {
    private:
        enum CommandTable
        {
            NOP = 0x00,
            SWRESET = 0x01,
            RDDID = 0x04,
            RDDST = 0x09,
            SLPIN = 0x10,
            SLPOUT = 0x11,
            PTLON = 0x12,
            NORON = 0x13,
            INVOFF = 0x20,
            INVON = 0x21,
            DISPOFF = 0x28,
            DISPON = 0x29,
            CASET = 0x2A,
            RASET = 0x2B,
            RAMWR = 0x2C,
            RAMRD = 0x2E,
            PTLAR = 0x30,
            COLMOD = 0x3A,
            MADCTL = 0x36,
            PORCTRL = 0xB2,
            GCTRL = 0xB7,
            VCOMS = 0xBB,
            LCMCTRL = 0xC0,
            VDVVRHEN = 0xC2,
            VRHS = 0xC3,
            VDVS = 0xC4,
            FRCTRL2 = 0xC6,
            PWCTRL1 = 0xD0,
            PVGAMCTRL = 0xE0,
            NVGAMCTRL = 0xE1,
        };
        struct MADCTLBits
        {
            uint8_t : 2;
            uint8_t MH : 1;
            uint8_t RGB : 1;
            uint8_t ML : 1;
            uint8_t MV : 1;
            uint8_t MX : 1;
            uint8_t MY : 1;
        };
        union MADCTLUnion
        {
            MADCTLBits Struct;
            uint8_t data;
        };
        iSpi &_spi = LcdSpiType::Get();
        iGpio &_dcPin = DcPinType::Get();
        iGpio &_resPin = ResPinType::Get();
        iGpio &_csPin = CsPinType::Get();
        uint16_t _width;
        uint16_t _height;
        bool _endOfSequence;
        Orientation _currentOrientation;
        uint16_t _orientationShiftX;
        uint16_t _orientationShiftY;
        FontType *_currentFont;
        void onWriteDone()
        {
            if (_endOfSequence)
            {
                _csPin.SetVal(true);
            }
        }
        void WriteCommand(uint8_t cmd)
        {
            while (_spi.IsBusy())
            {
            }
            _csPin.SetVal(false);
            _dcPin.SetVal(false);
            _spi.Write(&cmd, sizeof(cmd));
            while (_spi.IsBusy())
            {
            }
        }
        void WriteData(uint8_t *buff, uint32_t buffSize)
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
                _endOfSequence = buffSize > 0 ? false : true;
                _spi.Write(buff, chunk);
                buff += chunk;
            }
        }
        void WriteData(uint8_t data)
        {
            while (_spi.IsBusy())
            {
            }
            _csPin.SetVal(false);
            _dcPin.SetVal(true);
            _spi.Write(&data, sizeof(data));
            while (_spi.IsBusy())
            {
            }
        }
        void SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
        {
            uint16_t x_start = x0 + _orientationShiftX, x_end = x1 + _orientationShiftX;
            uint16_t y_start = y0 + _orientationShiftY, y_end = y1 + _orientationShiftY;

            /* Column Address set */
            WriteCommand(CASET);
            {
                uint8_t data[] = {(uint8_t)(x_start >> 8), (uint8_t)(x_start & 0xFF), (uint8_t)(x_end >> 8), (uint8_t)(x_end & 0xFF)};
                WriteData(data, sizeof(data));
                while (_spi.IsBusy())
                {
                }
            }

            /* Row Address set */
            WriteCommand(RASET);
            {
                uint8_t data[] = {(uint8_t)(y_start >> 8), (uint8_t)(y_start & 0xFF), (uint8_t)(y_end >> 8), (uint8_t)(y_end & 0xFF)};
                WriteData(data, sizeof(data));
                while (_spi.IsBusy())
                {
                }
            }
            /* Write to RAM */
            WriteCommand(RAMWR);
        }
        ST7789() : _width(0), _height(0), _endOfSequence(true), _currentOrientation(PORTRAIT), _orientationShiftX(0), _orientationShiftY(0), _currentFont((FontType *)&Font_11x18)
        {
        }

    public:
        static ST7789 &Get()
        {
            static ST7789 instance;
            return instance;
        }
        bool Init(int width, int height, iDelayer &delayer, Orientation orientation = PORTRAIT)
        {
            bool res = true;
            res &= _spi.Init(*this);
            res &= _dcPin.Init(iGpio::OutputPushPull);
            res &= _resPin.Init(iGpio::OutputPushPull);
            res &= _csPin.Init(iGpio::OutputPushPull);

            _width = width;
            _height = height;

            _csPin.SetVal(true);
            delayer.Delay(25);
            _resPin.SetVal(false);
            delayer.Delay(25);
            _resPin.SetVal(true);
            delayer.Delay(50);

            WriteCommand(COLMOD); //	Set color mode
            WriteData(0x55);      //    16bit Color mode

            WriteCommand(PORCTRL); //	Porch control
            {
                uint8_t data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
                WriteData(data, sizeof(data));
                while (_spi.IsBusy())
                {
                }
            }

            SetOrientation(orientation);

            WriteCommand(GCTRL); //	Gate Control
            WriteData(0x35);     //	Default value

            WriteCommand(VCOMS); //	VCOM setting
            WriteData(0x19);     //	0.725v (default 0.75v for 0x20)

            WriteCommand(LCMCTRL); //	LCMCTRL
            WriteData(0x2C);       //	Default value

            WriteCommand(VDVVRHEN); //	VDV and VRH command Enable
            {
                uint8_t data[] = {0x01, 0xff};
                WriteData(data, sizeof(data));
                while (_spi.IsBusy())
                {
                }
            }
            WriteCommand(VRHS); //	VRH set
            WriteData(0x12);    //	+-4.45v (defalut +-4.1v for 0x0B)

            WriteCommand(VDVS); //	VDV set
            WriteData(0x20);    //	Default value

            WriteCommand(FRCTRL2); //	Frame rate control in normal mode
            WriteData(0x0F);       //	Default value (60HZ)

            WriteCommand(PWCTRL1); //	Power control
            WriteData(0xA4);       //	Default value
            WriteData(0xA1);       //	Default value

            WriteCommand(PVGAMCTRL);
            {
                uint8_t data[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
                WriteData(data, sizeof(data));
                while (_spi.IsBusy())
                {
                }
            }

            WriteCommand(NVGAMCTRL);
            {
                uint8_t data[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
                WriteData(data, sizeof(data));
                while (_spi.IsBusy())
                {
                }
            }
            WriteCommand(INVON);  //	Inversion ON
            WriteCommand(SLPOUT); //	Out of sleep mode
            WriteCommand(NORON);  //	Normal Display on
            WriteCommand(DISPON); //	Main screen turned on

            delayer.Delay(50);
            FillColor(Black565); //	Fill with Black.

            return res;
        }
        virtual uint16_t Width()
        {
            return _width;
        }
        virtual uint16_t Height()
        {
            return _height;
        }
        inline virtual uint16_t GetColorFromRGB(uint8_t R, uint8_t G, uint8_t B)
        {
            return (((uint16_t)R & 0xF8) << 8) | (((uint16_t)G & 0xFC) << 3) | (((uint16_t)B >> 3));
        }
        virtual void SetOrientation(Orientation orientation)
        {
            WriteCommand(MADCTL);

            switch (orientation)
            {
            case PORTRAIT:
                WriteData(MADCTLUnion{{0, 0, 0, 0, 0, 0}}.data);
                _orientationShiftX = 0;
                _orientationShiftY = 0;
                break;
            case PORTRAIT_MIRROR:
                WriteData(MADCTLUnion{{1, 0, 1, 0, 1, 1}}.data);
                _orientationShiftX = 0;
                _orientationShiftY = 80;
                break;
            case LANDSCAPE:
                WriteData(MADCTLUnion{{1, 0, 0, 1, 1, 0}}.data);
                _orientationShiftX = 0;
                _orientationShiftY = 0;
                break;
            case LANDSCAPE_MIRROR:
                WriteData(MADCTLUnion{{0, 0, 1, 1, 0, 1}}.data);
                _orientationShiftX = 80;
                _orientationShiftY = 0;
                break;
            default:
                return;
            }
            _currentOrientation = orientation;
        }
        virtual void DrawPixel(uint16_t x, uint16_t y, uint16_t color)
        {
            if ((x < 0) || (x >= _width) ||
                (y < 0) || (y >= _height))
                return;

            SetAddressWindow(x, y, x, y);
            uint8_t data[] = {color >> 8, color & 0xFF};
            WriteData(data, sizeof(data));
        }
        virtual void FillWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
        {
            uint16_t tmp;
            if (x1 > x2)
            {
                tmp = x1;
                x1 = x2;
                x2 = tmp;
            }
            if (y1 > y2)
            {
                tmp = y1;
                y1 = y2;
                y2 = tmp;
            }
            if (x1 > _width - 1 || y1 > _height - 1)
                return;
            if (x2 > _width - 1)
                x2 = _width - 1;
            if (y2 > _height - 1)
                y2 = _height - 1;

            uint32_t len = (x2 - x1 + 1) * (y2 - y1 + 1); //количество закрашиваемых пикселей

            SetAddressWindow(x1, y1, x2, y2);

            uint16_t disp_buf[_width];

            color = (color >> 8) | ((color & 0xFF) << 8);

            int count;

            for (count = 0; count < len && count < _width; count++)
            {
                disp_buf[count] = color;
            }

            while (len)
            {
                WriteData((uint8_t *)disp_buf, len > count ? count * 2: len * 2);
                len -= count;
                while (_spi.IsBusy())
                {
                }
            }
        }
        virtual void DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *data)
        {
            if ((x >= _width) || (y >= _height))
                return;
            if ((x + width - 1) >= _width)
                return;
            if ((y + height - 1) >= _height)
                return;

            SetAddressWindow(x, y, x + width - 1, y + height - 1);
            WriteData(data, sizeof(uint16_t) * width * height);
        }
        virtual void FillColor(uint16_t color)
        {
            uint16_t i;
            SetAddressWindow(0, 0, _width - 1, _height - 1);

            uint16_t disp_buf[_width];

            color = (color >> 8) | ((color & 0xFF) << 8);

            for (i = 0; i < _width; i++)
            {
                disp_buf[i] = color;
            }

            for (i = 0; i < _height; i++)
            {
                WriteData((uint8_t *)disp_buf, sizeof(disp_buf));
                while (_spi.IsBusy())
                {
                }
            }
        }
        virtual void WriteChar(char ch, uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor)
        {
            uint32_t i, b, j;
            SetAddressWindow(x, y, x + _currentFont->width - 1, y + _currentFont->height - 1);

            for (i = 0; i < _currentFont->height; i++)
            {
                b = _currentFont->data[(ch - 32) * _currentFont->height + i];
                for (j = 0; j < _currentFont->width; j++)
                {
                    if ((b << j) & 0x8000)
                    {
                        uint8_t data[] = {(uint8_t)(color >> 8), (uint8_t)(color & 0xFF)};
                        WriteData(data, sizeof(data));
                    }
                    else
                    {
                        uint8_t data[] = {(uint8_t)(bgcolor >> 8), (uint8_t)(bgcolor & 0xFF)};
                        WriteData(data, sizeof(data));
                    }
                }
            }
        }
        virtual void WriteString(const char *str, uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor)
        {
            while (*str)
            {
                if (x + _currentFont->width >= _width)
                {
                    x = 0;
                    y += _currentFont->height;
                    if (y + _currentFont->height >= _height)
                    {
                        break;
                    }

                    if (*str == ' ')
                    {
                        // skip spaces in the beginning of the new line
                        str++;
                        continue;
                    }
                }
                WriteChar(*str, x, y, color, bgcolor);
                x += _currentFont->width;
                str++;
            }
        }
        virtual void SetFont(const FontType &font)
        {
            _currentFont = (FontType *)&font;
        }
        virtual bool InterfaceBusy()
        {
            return _spi.IsBusy();
        }
    };
}