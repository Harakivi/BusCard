#pragma once
#include <stdint.h>
#include "iTask.hpp"
#include "TaskManager.hpp"
#include "BoardButtons.hpp"
#include <initializer_list>
#include "Board.hpp"
#include "iGpio.hpp"

typedef Hardware::BusCard Board;

namespace Tasks
{
    template <uint32_t StackSize>
    class ButtonTask : public iTask
    {
    private:
        uint8_t taskStack[StackSize];
        TaskManager &_taskManager;
        Drivers::ActiveButtons _currButtState;
        std::initializer_list<Drivers::iButtons *> _buttSubscribers;

        template <Drivers::ButtonMask mask>
        class Button : public Hardware::GpioHandler
        {
            Drivers::ActiveButtons &_buttonState;

        public:
            Button(Drivers::ActiveButtons &buttonState) : _buttonState(buttonState){};
            virtual void Handler(bool value)
            {
                if constexpr (mask == Drivers::A)
                {
                    _buttonState.namedStruct.A = !value;
                }
                if constexpr (mask == Drivers::B)
                {
                    _buttonState.namedStruct.B = !value;
                }
                if constexpr (mask == Drivers::Select)
                {
                    _buttonState.namedStruct.Select = !value;
                }
                if constexpr (mask == Drivers::Start)
                {
                    _buttonState.namedStruct.Start = value;
                }
                if constexpr (mask == Drivers::Up)
                {
                    _buttonState.namedStruct.Up = !value;
                }
                if constexpr (mask == Drivers::Down)
                {
                    _buttonState.namedStruct.Down = !value;
                }
                if constexpr (mask == Drivers::Left)
                {
                    _buttonState.namedStruct.Left = !value;
                }
                if constexpr (mask == Drivers::Right)
                {
                    _buttonState.namedStruct.Right = !value;
                }
            }
        };

    public:
        ButtonTask(const char *name, std::initializer_list<Drivers::iButtons *> buttSubscribers) : iTask{name}, _taskManager(TaskManager::Get()), _buttSubscribers(buttSubscribers) {}
        virtual void Task()
        {
            Drivers::ActiveButtons lastButtState;
            lastButtState.data = 0;
            _currButtState.data = 0;

            Button<Drivers::A> _A = Button<Drivers::A>(_currButtState);
            Board::BtnA::Get().Init(Hardware::iGpio::GPIO_Modes::InputInterrupt, &_A, Hardware::iGpio::BothEdges);
            Board::BtnA::Get().SetPull(Hardware::iGpio::Pull_Modes::PullUp);

            Button<Drivers::B> _B = Button<Drivers::B>(_currButtState);
            Board::BtnB::Get().Init(Hardware::iGpio::GPIO_Modes::InputInterrupt, &_B, Hardware::iGpio::BothEdges);
            Board::BtnB::Get().SetPull(Hardware::iGpio::Pull_Modes::PullUp);

            Button<Drivers::Select> _Select = Button<Drivers::Select>(_currButtState);
            Board::BtnSelect::Get().Init(Hardware::iGpio::GPIO_Modes::InputInterrupt, &_Select, Hardware::iGpio::BothEdges);
            Board::BtnSelect::Get().SetPull(Hardware::iGpio::Pull_Modes::PullUp);
            
            Button<Drivers::Start> _Start = Button<Drivers::Start>(_currButtState);
            Board::BtnStart::Get().Init(Hardware::iGpio::GPIO_Modes::InputInterrupt, &_Start, Hardware::iGpio::BothEdges);

            Button<Drivers::Up> _Up = Button<Drivers::Up>(_currButtState);
            Board::BtnUp::Get().Init(Hardware::iGpio::GPIO_Modes::InputInterrupt, &_Up, Hardware::iGpio::BothEdges);
            Board::BtnUp::Get().SetPull(Hardware::iGpio::Pull_Modes::PullUp);

            Button<Drivers::Down> _Down = Button<Drivers::Down>(_currButtState);
            Board::BtnDown::Get().Init(Hardware::iGpio::GPIO_Modes::InputInterrupt, &_Down, Hardware::iGpio::BothEdges);
            Board::BtnDown::Get().SetPull(Hardware::iGpio::Pull_Modes::PullUp);

            Button<Drivers::Left> _Left = Button<Drivers::Left>(_currButtState);
            Board::BtnLeft::Get().Init(Hardware::iGpio::GPIO_Modes::InputInterrupt, &_Left, Hardware::iGpio::BothEdges);
            Board::BtnLeft::Get().SetPull(Hardware::iGpio::Pull_Modes::PullUp);

            Button<Drivers::Right> _Right = Button<Drivers::Right>(_currButtState);
            Board::BtnRight::Get().Init(Hardware::iGpio::GPIO_Modes::InputInterrupt, &_Right, Hardware::iGpio::BothEdges);
            Board::BtnRight::Get().SetPull(Hardware::iGpio::Pull_Modes::PullUp);

            for (;;)
            {
                if (_currButtState.data != lastButtState.data)
                {
                    for (auto buttSubscriber : _buttSubscribers)
                    {
                        buttSubscriber->ButtonStateChanged(_currButtState);
                    }

                    lastButtState = _currButtState;
                }
                _taskManager.Delay(50);
            }
        }
        virtual uint32_t GetStackSize()
        {
            return StackSize;
        }

        virtual uint8_t *GetTaskStack()
        {
            return taskStack;
        }
        void cliKeyHandle(uint8_t data)
        {
            switch (data)
            {
            case ('W'):
            case ('w'):
            {
                _currButtState.namedStruct.Up = 1;
            }
            break;
            case ('S'):
            case ('s'):
            {
                _currButtState.namedStruct.Down = 1;
            }
            break;
            case ('A'):
            case ('a'):
            {
                _currButtState.namedStruct.Left = 1;
            }
            break;
            case ('D'):
            case ('d'):
            {
                _currButtState.namedStruct.Right = 1;
            }
            break;
            case ('O'):
            case ('o'):
            {
                _currButtState.namedStruct.A = 1;
            }
            break;
            case ('K'):
            case ('k'):
            {
                _currButtState.namedStruct.B = 1;
            }
            break;
            case ('N'):
            case ('n'):
            {
                _currButtState.namedStruct.Start = 1;
            }
            break;
            case ('B'):
            case ('b'):
            {
                _currButtState.namedStruct.Select = 1;
            }
            break;

            default:
                break;
            }
        };
    };
}