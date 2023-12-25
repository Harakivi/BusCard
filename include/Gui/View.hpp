#pragma once
#include <stdint.h>
#include "Utils/GFXLib.hpp"

namespace Gui
{
    class View
    {
    public:
        uint32_t xPos;
        uint32_t yPos;
        uint32_t width;
        uint32_t height;
        Utils::GFX::FontType font;
    };
}