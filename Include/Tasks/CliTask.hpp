#pragma once
#include "TaskManager.hpp"
#include "Cli.hpp"

namespace Tasks
{
    template <uint32_t StackSize>
    class CliTask : public TaskBase
    {
        StaticTask_t taskTCB;
        StackType_t taskStack[StackSize];
        TaskManager *_taskManager;
        Drivers::Cli *_cliInstance;

    public:
        CliTask(Drivers::Cli *cliInstance) : _cliInstance(cliInstance)
        {
            _taskManager = TaskManager::Get();
        }
        virtual void Task()
        {
            uint32_t time = 0;
            for (;;)
            {
                time++;
                _cliInstance->Loop(time);
                _taskManager->TaskDelay(1);
            }
        }
        virtual const char *GetName()
        {
            return "CliTask";
        }
        virtual uint32_t GetStackSize()
        {
            return StackSize;
        }

        virtual StackType_t *GetTaskStack()
        {
            return taskStack;
        }

        virtual StaticTask_t *GetTaskTCB()
        {
            return &taskTCB;
        }
    };
}