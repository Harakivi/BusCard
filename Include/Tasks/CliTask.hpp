#pragma once
#include "Interfaces/iTask.hpp"
#include "TaskManager.hpp"
#include "Cli.hpp"
#include <stdint.h>

namespace Tasks
{
    template <uint32_t StackSize>
    class CliTask : public iTask
    {
        uint8_t taskStack[StackSize];
        TaskManager& _taskManager;
        Drivers::Cli& _cliInstance;
    public:
        CliTask(const char* name, Drivers::Cli& cliInstance):iTask{name},_taskManager(TaskManager::Get()), _cliInstance(cliInstance){}

        virtual void Task()
        {
            uint32_t time = 0;
            for (;;)
            {
                time++;
                _cliInstance.Loop(time);
                _taskManager.Delay(1);
            }
        }
        virtual uint32_t GetStackSize()
        {
            return StackSize;
        }

        virtual uint8_t* GetTaskStack()
        {
            return taskStack;
        }
    };
}