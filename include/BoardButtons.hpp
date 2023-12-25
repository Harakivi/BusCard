#pragma once
#include "stdint.h"

namespace Drivers
{

    enum ButtonMask
    {
        A = 0x1,
        B = 0x2,
        Select = 0x4,
        Start = 0x8,
        Up = 0x10,
        Down = 0x20,
        Left = 0x40,
        Right = 0x80,
    };

    struct ActiveButtonsStruct
    {
        uint8_t A : 1;
        uint8_t B : 1;
        uint8_t Select : 1;
        uint8_t Start : 1;
        uint8_t Up : 1;
        uint8_t Down : 1;
        uint8_t Left : 1;
        uint8_t Right : 1;
    };

    union ActiveButtons
    {
        ActiveButtonsStruct namedStruct;
        uint8_t data;
    };

    class iButtons
    {
    public:
        virtual void ButtonStateChanged(ActiveButtons butt) = 0;
    };

} // namespace Drivers
