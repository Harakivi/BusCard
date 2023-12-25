#pragma once
#include <stdint.h>
#include "iTask.hpp"

namespace Tasks
{
    class TaskManager : public Utils::iDelayer
    {
    private:
        bool _started;
        TaskManager() : _started(false){};

    public:
        static TaskManager &Get()
        {
            static TaskManager instance;
            return instance;
        }
        void Start();
        bool AddTask(iTask &instance, uint8_t taskPriority);
        bool IsStarted();
        void EnterCriticalSection();
        void ExitCriticalSection();
        virtual void Delay(uint32_t delay);
    };
}