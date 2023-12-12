#pragma once
#include "stdint.h"

namespace Tasks
{
    class TaskBase
    {
    protected:
        const char *_name;
    public:
        TaskBase(const char *name) : _name(name) {}
        virtual void Task() = 0;
        static void Starter(void *parameters)
        {
            if (parameters != nullptr)
            {
                ((TaskBase *)parameters)->Task();
            }
        }
        virtual const char *GetName()
        {
            return _name;
        }
        virtual uint32_t GetStackSize() = 0;
        virtual uint8_t *GetTaskStack() = 0;
    };

    class TaskManager
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
        bool AddTask(TaskBase &instance, uint8_t taskPriority);
        bool IsStarted();
        void TaskDelay(uint32_t delay);
    };
}

namespace Hardware
{
    class UartHandler
    {
    public:
        virtual void onByteReceived(uint8_t data) = 0;
    };

    class iUart
    {
    public:
        virtual bool Open(uint32_t BaudRate, void (*onByteReceived)(uint8_t) = nullptr) = 0;

        virtual bool Open(uint32_t BaudRate, UartHandler *handler) = 0;

        virtual bool Write(uint8_t *buffer, uint32_t count) = 0;

        virtual uint32_t GetReceived(uint8_t *buffer, uint32_t size) = 0;
    };

    class SpiHandler
    {
    public:
        virtual void onWriteDone() = 0;
    };

    class iSpi
    {
    public:
        virtual bool Init(SpiHandler& handler) = 0;
        virtual void Write(uint8_t *buff, uint16_t size) = 0;
        virtual bool IsBusy() = 0;
    };

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