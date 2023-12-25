#pragma once
#include <stdint.h>
#include <math.h>
#include "iLcd.hpp"

namespace Utils
{
    class GFX : public Drivers::iLcd
    {
    public:
        void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
        {
            if (x0 == x1 || y0 == y1)
            {
                int16_t tmp;
                if (x0 > x1)
                {
                    tmp = x0;
                    x0 = x1;
                    x1 = tmp;
                }
                if (y0 > y1)
                {
                    tmp = y0;
                    y0 = y1;
                    y1 = tmp;
                }
                if (x1 < 0 || x0 > Width() - 1)
                    return;
                if (y1 < 0 || y0 > Height() - 1)
                    return;
                if (x0 < 0)
                    x0 = 0;
                if (y0 < 0)
                    y0 = 0;
                FillWindow(x0, y0, x1, y1, color);
                return;
            }
            int16_t swap;
            uint16_t steep = abs(y1 - y0) > abs(x1 - x0);
            if (steep)
            {
                swap = x0;
                x0 = y0;
                y0 = swap;

                swap = x1;
                x1 = y1;
                y1 = swap;
            }

            if (x0 > x1)
            {
                swap = x0;
                x0 = x1;
                x1 = swap;

                swap = y0;
                y0 = y1;
                y1 = swap;
            }

            int16_t dx, dy;
            dx = x1 - x0;
            dy = abs(y1 - y0);

            int16_t err = dx / 2;
            int16_t ystep;

            if (y0 < y1)
            {
                ystep = 1;
            }
            else
            {
                ystep = -1;
            }

            for (; x0 <= x1; x0++)
            {
                if (steep)
                {
                    DrawPixel(y0, x0, color);
                }
                else
                {
                    DrawPixel(x0, y0, color);
                }
                err -= dy;
                if (err < 0)
                {
                    y0 += ystep;
                    err += dx;
                }
            }
        }
        void DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
        {
            DrawLine(x0, y0, x1, y0, color);
            DrawLine(x0, y0, x0, y1, color);
            DrawLine(x0, y1, x1, y1, color);
            DrawLine(x1, y0, x1, y1, color);
        }

        void DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
        {
            int16_t f = 1 - r;
            int16_t ddF_x = 1;
            int16_t ddF_y = -2 * r;
            int16_t x = 0;
            int16_t y = r;

            DrawPixel(x0, y0 + r, color);
            DrawPixel(x0, y0 - r, color);
            DrawPixel(x0 + r, y0, color);
            DrawPixel(x0 - r, y0, color);

            while (x < y)
            {
                if (f >= 0)
                {
                    y--;
                    ddF_y += 2;
                    f += ddF_y;
                }
                x++;
                ddF_x += 2;
                f += ddF_x;

                DrawPixel(x0 + x, y0 + y, color);
                DrawPixel(x0 - x, y0 + y, color);
                DrawPixel(x0 + x, y0 - y, color);
                DrawPixel(x0 - x, y0 - y, color);

                DrawPixel(x0 + y, y0 + x, color);
                DrawPixel(x0 - y, y0 + x, color);
                DrawPixel(x0 + y, y0 - x, color);
                DrawPixel(x0 - y, y0 - x, color);
            }
        }

        void DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
        {
            uint8_t i;

            /* Check input parameters */
            if (x >= Width() ||
                y >= Height())
            {
                /* Return error */
                return;
            }

            /* Check width and height */
            if ((x + width) >= Width())
            {
                width = Width() - x;
            }
            if ((y + height) >= Height())
            {
                height = Height() - y;
            }

            /* Draw lines */
            for (i = 0; i <= height; i++)
            {
                /* Draw lines */
                DrawLine(x, y + i, x + width, y + i, color);
            }
        }

        void DrawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
        {
            /* Draw lines */
            DrawLine(x0, y0, x1, y1, color);
            DrawLine(x1, y1, x2, y2, color);
            DrawLine(x2, y2, x0, y0, color);
        }

        void DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
        {
            int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
                    yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
                    curpixel = 0;

            deltax = abs(x2 - x1);
            deltay = abs(y2 - y1);
            x = x1;
            y = y1;

            if (x2 >= x1)
            {
                xinc1 = 1;
                xinc2 = 1;
            }
            else
            {
                xinc1 = -1;
                xinc2 = -1;
            }

            if (y2 >= y1)
            {
                yinc1 = 1;
                yinc2 = 1;
            }
            else
            {
                yinc1 = -1;
                yinc2 = -1;
            }

            if (deltax >= deltay)
            {
                xinc1 = 0;
                yinc2 = 0;
                den = deltax;
                num = deltax / 2;
                numadd = deltay;
                numpixels = deltax;
            }
            else
            {
                xinc2 = 0;
                yinc1 = 0;
                den = deltay;
                num = deltay / 2;
                numadd = deltax;
                numpixels = deltay;
            }

            for (curpixel = 0; curpixel <= numpixels; curpixel++)
            {
                DrawLine(x, y, x3, y3, color);

                num += numadd;
                if (num >= den)
                {
                    num -= den;
                    x += xinc1;
                    y += yinc1;
                }
                x += xinc2;
                y += yinc2;
            }
        }

        void DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
        {
            int16_t f = 1 - r;
            int16_t ddF_x = 1;
            int16_t ddF_y = -2 * r;
            int16_t x = 0;
            int16_t y = r;

            DrawLine(x0 - r, y0, x0 + r, y0, color);

            while (x < y)
            {
                if (f >= 0)
                {
                    y--;
                    ddF_y += 2;
                    f += ddF_y;
                }
                x++;
                ddF_x += 2;
                f += ddF_x;

                DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
                DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

                DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
                DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
            }
        }
    };
}