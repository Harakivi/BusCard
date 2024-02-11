#pragma once
#include <stdint.h>
#include "View.hpp"
#include "Utils/GFXLib.hpp"
#include "BoardButtons.hpp"

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
        Control() : navigation{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, false}, view{0, 0, 0, 0, Utils::GFX::Font_7x10}, _parentControl(*(Control *)this), _onNavigateButton(Drivers::ButtonMask::None) {}

    protected:
        Control &_parentControl;
        Drivers::ButtonMask _onNavigateButton;

    public:
        Control(Control &parentControl) : navigation{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, false}, view{0, 0, 0, 0, Utils::GFX::Font_7x10}, _parentControl(parentControl), _onNavigateButton(Drivers::ButtonMask::None){}

        virtual void Draw(Utils::GFX &gfx)
        {
        }
        virtual void Processing(Utils::GFX &gfx, Control **activePtr)
        {
        }
        virtual Control &ButtonStateChanged(Drivers::ActiveButtons butt, Utils::GFX &gfx)
        {
            using namespace Drivers;
            if (butt.data == ButtonMask::Up && navigation.ToUp)
            {
                _onNavigateButton = ButtonMask::Up;
                navigation.Active = false;
                navigation.ToUp->navigation.Active = true;
                Draw(gfx);
                navigation.ToUp->Draw(gfx);
                return *navigation.ToUp;
            }
            if (butt.data == ButtonMask::Down && navigation.ToDown)
            {
                _onNavigateButton = ButtonMask::Down;
                navigation.Active = false;
                navigation.ToDown->navigation.Active = true;
                Draw(gfx);
                navigation.ToDown->Draw(gfx);
                return *navigation.ToDown;
            }
            if (butt.data == ButtonMask::Left && navigation.ToLeft)
            {
                _onNavigateButton = ButtonMask::Left;
                navigation.Active = false;
                navigation.ToLeft->navigation.Active = true;
                Draw(gfx);
                navigation.ToLeft->Draw(gfx);
                return *navigation.ToLeft;
            }
            if (butt.data == ButtonMask::Right && navigation.ToRight)
            {
                _onNavigateButton = ButtonMask::Right;
                navigation.Active = false;
                navigation.ToRight->navigation.Active = true;
                Draw(gfx);
                navigation.ToRight->Draw(gfx);
                return *navigation.ToRight;
            }
            if ((butt.data == ButtonMask::A || butt.data == ButtonMask::Start) && navigation.Enter)
            {
                if(butt.namedStruct.A)
                    _onNavigateButton = ButtonMask::A;
                if(butt.namedStruct.Start)
                    _onNavigateButton = ButtonMask::Start;
                navigation.Active = false;
                navigation.Enter->navigation.Active = true;
                Draw(gfx);
                navigation.Enter->Draw(gfx);
                return *navigation.Enter;
            }
            if ((butt.data == ButtonMask::B || butt.data == ButtonMask::Select) && navigation.Exit)
            {
                if(butt.namedStruct.B)
                    _onNavigateButton = ButtonMask::B;
                if(butt.namedStruct.Select)
                    _onNavigateButton = ButtonMask::Select;
                navigation.Active = false;
                navigation.Exit->navigation.Active = true;
                Draw(gfx);
                navigation.Exit->Draw(gfx);
                return *navigation.Exit;
            }
            return *this;
        }

        virtual Drivers::ButtonMask GetOnNavigateButton()
        {
            Drivers::ButtonMask ret = _onNavigateButton;
            _onNavigateButton = Drivers::ButtonMask::None;
            return ret;
        }

        virtual void ResetNavigation()
        {
            navigation.ToUp = nullptr;
            navigation.ToDown = nullptr;
            navigation.ToLeft = nullptr;
            navigation.ToRight = nullptr;
            navigation.Enter = nullptr;
            navigation.Exit = nullptr;
        }

        virtual void ReDrawChild(Utils::GFX &gfx)
        {
        }
    };
}