#pragma once
#include <stdint.h>
#include "Utils/GFXLib.hpp"

namespace Gui
{
    class Window
    {
    public:
        Window() {}
        virtual void ReDrawChild(Utils::GFX &gfx) = 0;
    };
}