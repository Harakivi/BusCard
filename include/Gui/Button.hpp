#pragma once
#include <stdint.h>
#include "View.hpp"
#include "Control.hpp"
#include "BoardButtons.hpp"
#include "string.h"

namespace Gui
{
    class Button : public Control
    {
        typedef Utils::GFX::RGB565Colors Colors;
        Colors _textColor;
        Colors _bgColor;
        Colors _frameColor;
        Colors _activeColor;
        const char *_text;
        size_t _textLen;
        TextAlign _textAlign;
        Button() {}

    public:
        Button(const char *text, Colors textColor = Colors::Yellow565, Colors bgColor = Colors::Black565, Colors frameColor = Colors::Brown565)
            : _textColor(textColor),
              _bgColor(bgColor),
              _frameColor(frameColor),
              _activeColor(Colors::Orange565),
              _text(text),
              _textAlign(TextAlign::Left)
        {
            _textLen = strlen(_text);
        }
        void SetTextAlign(TextAlign textAlign)
        {
            _textAlign = textAlign;
        }

        void SetText(const char *text)
        {
            _text = text;
            _textLen = strlen(_text);
        }
        virtual void Draw(Utils::GFX &gfx)
        {
            gfx.DrawFilledRectangle(view.xPos + 2, view.yPos + 2, view.width - 2, view.height - 2, _bgColor);
            gfx.SetFont(view.font);
            uint16_t textPos = view.xPos + 2;

            switch (_textAlign)
            {
            case Right:
                textPos = ((view.xPos + view.width) - (_textLen * view.font.width)) - 2;
                break;
            case Center:
                textPos = ((view.xPos + view.width) - (_textLen * view.font.width) - ((view.width / 2) - (_textLen * view.font.width) / 2));
                break;
            case Left:
            default:
                break;
            }

            gfx.WriteString(_text, textPos, view.yPos + 2, _textColor, _bgColor);
            if (!navigation.Active)
            {
                gfx.DrawRectangle(view.xPos, view.yPos, view.xPos + view.width, view.yPos + view.height, _frameColor);
                gfx.DrawRectangle(view.xPos + 1, view.yPos + 1, (view.xPos + view.width) - 1, (view.yPos + view.height) - 1, _frameColor);
            }
            else
            {
                gfx.DrawRectangle(view.xPos, view.yPos, view.xPos + view.width, view.yPos + view.height, _activeColor);
                gfx.DrawRectangle(view.xPos + 1, view.yPos + 1, (view.xPos + view.width) - 1, (view.yPos + view.height) - 1, _activeColor);
            }
        }
    };
}