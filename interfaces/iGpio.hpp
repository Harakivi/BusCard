#pragma once

namespace Hardware
{
    class GpioHandler
    {
    public:
        virtual void Handler(bool value) = 0;
    };

    class iGpio
    {
    public:
        enum GPIO_Modes
        {
            Input = 0,
            OutputPushPull,
            OutputOpenDrain,
            InputInterrupt
        };


        enum Interrupt_Modes
        {
            None = 0,
            RisingEdge,
            FallingEdge,
            BothEdges,
        };

        enum Pull_Modes
        {
            PullNone = 0,
            PullUp,
            PullDown,
        };

        virtual bool Init(iGpio::GPIO_Modes mode, GpioHandler* handler = nullptr, iGpio::Interrupt_Modes edge = None) = 0;
        virtual inline bool GetVal() = 0;
        virtual inline void SetVal(bool val) = 0;
        virtual void SetPull(Pull_Modes pull) = 0;
    };
}