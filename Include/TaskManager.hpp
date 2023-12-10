#pragma once
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>

class TaskBase
{
protected:
    TaskHandle_t _taskHandle;
public:
    virtual void Task() = 0;
    virtual const char * GetName() = 0;
    virtual void SetTaskHandle(TaskHandle_t taskHandle)
    {
        _taskHandle = taskHandle;
    }
    static void Starter(void *parameters)
    {
        if (parameters != nullptr)
        {
            ((TaskBase *)parameters)->Task();
        }
    }
    virtual uint32_t GetStackSize() = 0;
    virtual StackType_t *GetTaskStack() = 0;
    virtual StaticTask_t *GetTaskTCB() = 0;
};

class TaskManager
{
private:
    bool _started;
    TaskManager() : _started(false){};

public:
    static TaskManager *Get()
    {
        static TaskManager instance;
        return &instance;
    }
    void Start();
    bool AddTask(TaskBase *instance, uint8_t taskPriority);
    bool IsStarted();
    void TaskDelay(TickType_t delay);
};