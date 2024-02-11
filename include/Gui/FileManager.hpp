#pragma once
#include <stdint.h>
#include "View.hpp"
#include "Control.hpp"
#include "Button.hpp"
#include "Text.hpp"
#include "BoardButtons.hpp"
#include <string.h>
#include "fatfs/fatfs.h"
#include <string.h>
#include <cstdio>

namespace Gui
{
    class FileManager : public Control
    {
        char currDir[255];

        Text dir = Text(currDir, *this);
        class File : public Button
        {
            typedef Utils::GFX::RGB565Colors Colors;

        public:
            FILINFO fileInfo;
            File(Control &parentControl) : Button(fileInfo.fname, parentControl, Colors::Yellow565, Colors::Black565, Colors::Black565), fileInfo{0} {}
        };

        File files[8] = {File(*this),
                         File(*this),
                         File(*this),
                         File(*this),
                         File(*this),
                         File(*this),
                         File(*this),
                         File(*this)};

        char *_selectedName;

        void GoToUpperDir()
        {
            uint32_t lastFSlash = 0;
            for (uint32_t i = 0; i < sizeof(currDir) && currDir[i]; i++)
            {
                if (currDir[i] == '/')
                {
                    lastFSlash = i;
                }
            }
            if (lastFSlash)
                currDir[lastFSlash] = 0;
        }

    public:
        FileManager(Control &parentControl)
            : Control(parentControl),
              currDir(":"),
              _selectedName(nullptr)
        {
        }

        virtual void ReDrawChild(Utils::GFX &gfx)
        {
            dir.Draw(gfx);
            for (uint32_t i = 0; i < (sizeof(files) / sizeof(File)); i++)
            {
                files[i].Draw(gfx);
            }
        }

        virtual char *GetSelectedName()
        {
            return _selectedName;
        }

        virtual void Processing(Utils::GFX &gfx, Control **activePtr)
        {
            using namespace Drivers;
            for (uint32_t i = 0; i < (sizeof(files) / sizeof(File)); i++)
            {
                switch (files[i].GetOnNavigateButton())
                {
                case ButtonMask::A:
                {
                    if ((files[i].fileInfo.fattrib & AM_DIR) > 0)
                    {
                        strcat(currDir, "/");
                        strcat(currDir, files[i].fileInfo.fname);
                        break;
                    }
                    else
                    {
                        *activePtr = &_parentControl;
                        _selectedName = files[i].fileInfo.fname;
                        _onNavigateButton = ButtonMask::A;
                        return;
                    }
                }
                break;
                case ButtonMask::B:
                {
                    if (currDir[1] != 0)
                        GoToUpperDir();
                    else
                    {
                        *activePtr = &_parentControl;
                        _onNavigateButton = ButtonMask::B;
                        return;
                    }
                }
                break;

                default:
                    break;
                }
            }

            dir.view = Gui::View{0, 15, 200, 20, gfx.Font_11x18};
            FATFS FatFs; // Fatfs handle
            DIR dp;

            f_mount(&FatFs, "", 1); // 1=mount now

            f_findfirst(&dp, &files[0].fileInfo, currDir + 1, "*");

            for (uint8_t i = 1; i < (sizeof(files) / sizeof(File)) && f_findnext(&dp, &files[i].fileInfo) == FR_OK && &files[i].fileInfo.fname[0]; i++)
            {
            }

            gfx.FillColor(gfx.Black565);

            for (uint32_t i = 0; i < (sizeof(files) / sizeof(File)); i++)
            {
                files[i].ResetNavigation();
                files[i].navigation.Exit = this;
                files[i].navigation.Enter = this;
                files[i].view = Gui::View{0, 25 * i + 40, 200, 20, gfx.Font_11x18};
                if (i > 0 && files[i].fileInfo.fname[0] != 0)
                {
                    files[i].navigation.ToUp = &files[i - 1];
                }
                if (i < (sizeof(files) - 1) && files[i].fileInfo.fname[0] != 0 && files[i + 1].fileInfo.fname[0] != 0)
                {
                    files[i].navigation.ToDown = &files[i + 1];
                }
                if ((i == (sizeof(files) / sizeof(File)) - 1 || files[i + 1].fileInfo.fname[0] == 0) && i != 0 && files[0].navigation.ToUp == nullptr)
                {
                    files[0].navigation.ToUp = &files[i];
                    files[i].navigation.ToDown = &files[0];
                }
            }

            if (files[0].fileInfo.fname)
                files[0].navigation.Active = true;

            ReDrawChild(gfx);

            *activePtr = &files[0];
        }

        virtual Control &ButtonStateChanged(Drivers::ActiveButtons butt, Utils::GFX &gfx)
        {
            return *this;
        }
    };
}