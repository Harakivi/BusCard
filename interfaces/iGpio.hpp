#pragma once

namespace Hardware
{
    class iGpio
    {
    public:
        enum GPIO_Modes
        {
            Input,
            OutputPushPull,
            OutputOpenDrain,
        };

        enum Pull_Modes
        {
            PullNone = 0,
            PullUp,
            PullDown,
        };

        virtual bool Init(GPIO_Modes mode) = 0;
        virtual bool GetVal() = 0;
        virtual void SetVal(bool val) = 0;
        virtual void SetPull(Pull_Modes pull) = 0;
    };
}