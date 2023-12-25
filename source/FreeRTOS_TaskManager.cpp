#include "iTask.hpp"
#include "TaskManager.hpp"
#include "FreeRTOS.h"
#include "task.h"

using namespace Tasks;

void TaskManager::Start()
{
  _started = true;
  vTaskStartScheduler();
}

bool TaskManager::AddTask(iTask &instance, uint8_t taskPriority)
{
  StaticTask_t *taskTCB = (StaticTask_t *)instance.GetTaskStack();
  StackType_t *taskStack = (StackType_t *)(instance.GetTaskStack() + sizeof(StaticTask_t));
  uint32_t stackDeppth = ((instance.GetStackSize() - sizeof(StaticTask_t))) / sizeof(StackType_t);
  return xTaskCreateStatic(iTask::Starter,
                           instance.GetName(),
                           stackDeppth,
                           &instance,
                           taskPriority,
                           taskStack,
                           taskTCB) != nullptr;
}

bool TaskManager::IsStarted()
{
  return _started;
}

void TaskManager::EnterCriticalSection()
{
  portENTER_CRITICAL();
}
void TaskManager::ExitCriticalSection()
{
  portEXIT_CRITICAL();
}

void TaskManager::Delay(TickType_t delay)
{
  vTaskDelay(delay);
}

extern "C" void xPortSysTickHandler(void);

extern "C" void HAL_IncTick(void);

extern "C" void SysTick_Handler(void)
{
  if (Tasks::TaskManager::Get().IsStarted())
    xPortSysTickHandler();

  HAL_IncTick();
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
}

extern "C" void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                              StackType_t **ppxIdleTaskStackBuffer,
                                              uint32_t *pulIdleTaskStackSize)
{
  static const uint32_t _pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
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
