#pragma once
#include <stdint.h>
#include "View.hpp"
#include "Utils/GFXLib.hpp"
#include "BoardButtons.hpp"
#include "Window.hpp"

namespace Gui
{
    enum TextAlign
    {
        Left = 0,
        Center,
        Right,
    };

    class Control
    {
    public:
        class Navigation
        {
        public:
            Control *ToUp;
            Control *ToDown;
            Control *ToLeft;
            Control *ToRight;
            Control *Enter;
            Control *Exit;
            bool Active;
        };
        Navigation navigation;
        View view;
        Control() : navigation{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, false}, view{0, 0, 0, 0, Utils::GFX::Font_7x10}, _parentWindow(*(Window *)this) {}

    protected:
        Window &_parentWindow;

    public:
        Control(Window &parentWindow) : navigation{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, false}, view{0, 0, 0, 0, Utils::GFX::Font_7x10}, _parentWindow(parentWindow) {}

        virtual void Draw(Utils::GFX &gfx)
        {
        }
        virtual void Processing(Utils::GFX &gfx, Control **activePtr)
        {
        }
        virtual Control &ButtonStateChanged(Drivers::ActiveButtons butt, Utils::GFX &gfx)
        {
            if (!navigation.Active)
                return *this;
            if (butt.namedStruct.Up && navigation.ToUp)
            {
                navigation.Active = false;
                navigation.ToUp->navigation.Active = true;
                Draw(gfx);
                navigation.ToUp->Draw(gfx);
                return *navigation.ToUp;
            }
            if (butt.namedStruct.Down && navigation.ToDown)
            {
                navigation.Active = false;
                navigation.ToDown->navigation.Active = true;
                Draw(gfx);
                navigation.ToDown->Draw(gfx);
                return *navigation.ToDown;
            }
            if (butt.namedStruct.Left && navigation.ToLeft)
            {
                navigation.Active = false;
                navigation.ToLeft->navigation.Active = true;
                Draw(gfx);
                navigation.ToLeft->Draw(gfx);
                return *navigation.ToLeft;
            }
            if (butt.namedStruct.Right && navigation.ToRight)
            {
                navigation.Active = false;
                navigation.ToRight->navigation.Active = true;
                Draw(gfx);
                navigation.ToRight->Draw(gfx);
                return *navigation.ToRight;
            }
            if ((butt.namedStruct.A || butt.namedStruct.Start) && navigation.Enter)
            {
                navigation.Active = false;
                navigation.Enter->navigation.Active = true;
                Draw(gfx);
                navigation.Enter->Draw(gfx);
                return *navigation.Enter;
            }
            if ((butt.namedStruct.B || butt.namedStruct.Select) && navigation.Exit)
            {
                navigation.Active = false;
                navigation.Exit->navigation.Active = true;
                Draw(gfx);
                navigation.Exit->Draw(gfx);
                return *navigation.Exit;
            }
            return *this;
        }
    };
}