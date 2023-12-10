#include "TaskManager.hpp"

void TaskManager::Start()
{
    _started = true;
    vTaskStartScheduler();
}

bool TaskManager::AddTask(TaskBase *instance, uint8_t taskPriority)
{
  TaskHandle_t taskHandle = xTaskCreateStatic(TaskBase::Starter,
                                              instance->GetName(),
                                              instance->GetStackSize(),
                                              instance,
                                              taskPriority,
                                              instance->GetTaskStack(),
                                              instance->GetTaskTCB());
  if(taskHandle != nullptr)
  {
    instance->SetTaskHandle(taskHandle);
    return true;
  }
  return false;
}

bool TaskManager::IsStarted()
{
    return _started;
}

void TaskManager::TaskDelay(TickType_t delay)
{
  vTaskDelay(delay);
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
}

extern "C" void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                              StackType_t **ppxIdleTaskStackBuffer,
                                              uint32_t *pulIdleTaskStackSize)
{
  static const uint32_t _pulIdleTaskStackSize =configMINIMAL_STACK_SIZE;
  static StaticTask_t _ppxIdleTaskTCB;
  static StackType_t _ppxIdleTaskStack[_pulIdleTaskStackSize];
  *ppxIdleTaskTCBBuffer = &_ppxIdleTaskTCB;
  *ppxIdleTaskStackBuffer = _ppxIdleTaskStack;
  *pulIdleTaskStackSize = _pulIdleTaskStackSize;
}

extern "C" void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                               StackType_t **ppxTimerTaskStackBuffer,
                                               uint32_t *pulTimerTaskStackSize)
{
  static const uint32_t _pulTimerTaskStackSize = configMINIMAL_STACK_SIZE;
  static StaticTask_t _ppxTimerTaskTCB;
  static StackType_t _pulTimerTaskStack[_pulTimerTaskStackSize];
  *ppxTimerTaskTCBBuffer = &_ppxTimerTaskTCB;
  *ppxTimerTaskStackBuffer = _pulTimerTaskStack;
  *pulTimerTaskStackSize = _pulTimerTaskStackSize;
}
