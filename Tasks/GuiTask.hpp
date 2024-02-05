#pragma once
#include <stdint.h>
#include "iTask.hpp"
#include "TaskManager.hpp"
#include "Gui/Button.hpp"
#include "MainWindow.hpp"

namespace Tasks
{
    template <uint32_t StackSize>
    class GuiTask : public iTask, public Drivers::iButtons
    {
    private:
        uint8_t taskStack[StackSize];
        TaskManager &_taskManager;
        Utils::GFX &_lcd;
        Gui::MainWindow mainWindow;
        Gui::Control *activeControl = &mainWindow;

    public:
        GuiTask(const char *name, Utils::GFX &lcd) : iTask{name}, _taskManager(TaskManager::Get()), _lcd(lcd) {}

        virtual void Task()
        {
            for (;;)
            {
                activeControl->Processing(_lcd, &activeControl);
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

        virtual void ButtonStateChanged(Drivers::ActiveButtons butt)
        {
            activeControl = &activeControl->ButtonStateChanged(butt, _lcd);
        }
    };
}