#pragma once
#include <stdint.h>
#include "iTask.hpp"
#include "TaskManager.hpp"
#include "BoardButtons.hpp"
#include <initializer_list>
#include "Board.hpp"

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

    public:
        ButtonTask(const char *name, std::initializer_list<Drivers::iButtons *> buttSubscribers) : iTask{name}, _taskManager(TaskManager::Get()), _buttSubscribers(buttSubscribers) {}
        virtual void Task()
        {
            Drivers::ActiveButtons lastButtState;
            lastButtState.data = 0;
            _currButtState.data = 0;

            Board::BtnStart::Get().Init(Hardware::iGpio::GPIO_Modes::Input);
            Board::BtnSelect::Get().Init(Hardware::iGpio::GPIO_Modes::Input);
            Board::BtnSelect::Get().SetPull(Hardware::iGpio::Pull_Modes::PullUp);
            Board::BtnUp::Get().Init(Hardware::iGpio::GPIO_Modes::Input);
            Board::BtnUp::Get().SetPull(Hardware::iGpio::Pull_Modes::PullUp);
            Board::BtnDown::Get().Init(Hardware::iGpio::GPIO_Modes::Input);
            Board::BtnDown::Get().SetPull(Hardware::iGpio::Pull_Modes::PullUp);
            Board::BtnLeft::Get().Init(Hardware::iGpio::GPIO_Modes::Input);
            Board::BtnLeft::Get().SetPull(Hardware::iGpio::Pull_Modes::PullUp);
            Board::BtnRight::Get().Init(Hardware::iGpio::GPIO_Modes::Input);
            Board::BtnRight::Get().SetPull(Hardware::iGpio::Pull_Modes::PullUp);
            Board::BtnA::Get().Init(Hardware::iGpio::GPIO_Modes::Input);
            Board::BtnA::Get().SetPull(Hardware::iGpio::Pull_Modes::PullUp);
            Board::BtnB::Get().Init(Hardware::iGpio::GPIO_Modes::Input);
            Board::BtnB::Get().SetPull(Hardware::iGpio::Pull_Modes::PullUp);

            Hardware::iGpio &_A = Board::BtnA::Get();
            Hardware::iGpio &_B = Board::BtnB::Get();
            Hardware::iGpio &_Select = Board::BtnSelect::Get();
            Hardware::iGpio &_Start = Board::BtnStart::Get();
            Hardware::iGpio &_Up = Board::BtnUp::Get();
            Hardware::iGpio &_Down = Board::BtnDown::Get();
            Hardware::iGpio &_Left = Board::BtnLeft::Get();
            Hardware::iGpio &_Right = Board::BtnRight::Get();
            for (;;)
            {
                _currButtState.namedStruct.A = !_A.GetVal();
                _currButtState.namedStruct.B = !_B.GetVal();
                _currButtState.namedStruct.Select = !_Select.GetVal();
                _currButtState.namedStruct.Start = _Start.GetVal();
                _currButtState.namedStruct.Up = !_Up.GetVal();
                _currButtState.namedStruct.Down = !_Down.GetVal();
                _currButtState.namedStruct.Left = !_Left.GetVal();
                _currButtState.namedStruct.Right = !_Right.GetVal();
                if (_currButtState.data != lastButtState.data)
                {
                    for (auto buttSubscriber : _buttSubscribers)
                    {
                        buttSubscriber->ButtonStateChanged(_currButtState);
                    }

                    lastButtState = _currButtState;
                }
                _taskManager.Delay(100);
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