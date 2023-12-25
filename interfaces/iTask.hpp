#pragma once
#include <stdint.h>
#include "iDelayer.hpp"

namespace Tasks
{
    class iTask
    {
    protected:
        const char *_name;
    public:
        iTask(const char *name) : _name(name) {}
        virtual void Task() = 0;
        static void Starter(void *parameters)
        {
            if (parameters != nullptr)
            {
                ((iTask *)parameters)->Task();
            }
        }
        virtual const char *GetName()
        {
            return _name;
        }
        virtual uint32_t GetStackSize() = 0;
        virtual uint8_t *GetTaskStack() = 0;
    };
}