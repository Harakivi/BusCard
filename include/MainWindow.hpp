#pragma once
#include <stdint.h>
#include "Gui/Control.hpp"
#include "BoardButtons.hpp"
#include "nes.hpp"
#include "Gui/FileManager.hpp"

namespace Gui
{
    class MainWindow : public Control
    {
        Gui::Button btn1 = Gui::Button("NES emulator", *this);
        Gui::Button btn2 = Gui::Button("FileManager", *this);
        Gui::Button btn3 = Gui::Button("TestBtn3", *this);
        Gui::Button btn4 = Gui::Button("TestBtn4", *this);
        Gui::FileManager fileManager = Gui::FileManager(*this);
        NES_EMU nes = NES_EMU(*this);

    public:
        MainWindow()
        {
        }
        virtual void Processing(Utils::GFX &gfx, Control **activePtr)
        {
            gfx.FillColor(gfx.Black565);
            btn1.view = Gui::View{45, 140, 150, 20, gfx.Font_11x18};
            btn2.view = Gui::View{45, 165, 150, 20, gfx.Font_11x18};
            btn3.view = Gui::View{45, 190, 150, 20, gfx.Font_11x18};
            btn4.view = Gui::View{45, 215, 150, 20, gfx.Font_11x18};
            btn1.SetTextAlign(TextAlign::Center);
            btn2.SetTextAlign(TextAlign::Center);
            btn3.SetTextAlign(TextAlign::Center);
            btn4.SetTextAlign(TextAlign::Center);
            btn1.navigation.ToDown = &btn2;
            btn2.navigation.ToDown = &btn3;
            btn3.navigation.ToDown = &btn4;
            btn4.navigation.ToDown = &btn1;
            btn1.navigation.ToUp = &btn4;
            btn2.navigation.ToUp = &btn1;
            btn3.navigation.ToUp = &btn2;
            btn4.navigation.ToUp = &btn3;

            btn1.navigation.Enter = &nes;
            btn1.navigation.Active = true;
            btn2.navigation.Enter = &fileManager;
            
            ReDrawChild(gfx);

            *activePtr = &btn1;
        }

        virtual void ReDrawChild(Utils::GFX &gfx)
        {
            btn1.Draw(gfx);
            btn2.Draw(gfx);
            btn3.Draw(gfx);
            btn4.Draw(gfx);
        }

        virtual Control &ButtonStateChanged(Drivers::ActiveButtons butt, Utils::GFX &gfx)
        {
            return *this;
        }
    };

}