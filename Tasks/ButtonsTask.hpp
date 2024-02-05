#pragma once
#include <stdint.h>
#include "iTask.hpp"
#include "TaskManager.hpp"
#include "BoardButtons.hpp"
#include <initializer_list>

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
                _currButtState.data = 0;
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