#include "Board.hpp"
#include "iTask.hpp"
#include "iLcd.hpp"
#include "TaskManager.hpp"
#include "Cli.hpp"
#include "CliTask.hpp"
#include "Gui/Button.hpp"
#include "BoardButtons.hpp"
#include "MainWindow.hpp"

typedef Tasks::CliTask<256 * 4> CliTask;
typedef Hardware::BusCard Board;
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

  template <uint32_t StackSize>
  class ButtonTask : public iTask
  {
  private:
    uint8_t taskStack[StackSize];
    TaskManager &_taskManager;
    Drivers::ActiveButtons _currButtState;
    Drivers::iButtons &_buttSubscriber;

  public:
    ButtonTask(const char *name, Drivers::iButtons &buttSubscriber) : iTask{name}, _taskManager(TaskManager::Get()), _buttSubscriber(buttSubscriber) {}
    virtual void Task()
    {
      Drivers::ActiveButtons lastButtState;
      lastButtState.data = 0;
      _currButtState.data = 0;
      for (;;)
      {
        if (_currButtState.data != lastButtState.data)
        {
          _buttSubscriber.ButtonStateChanged(_currButtState);
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

typedef Tasks::GuiTask<256 * 6> GuiTask;
typedef Tasks::ButtonTask<256 * 6> ButtonTask;

int main()
{
  static Drivers::Cli cli = Drivers::Cli(Board::cliUart);
  static CliTask cliTask = CliTask("CliTask", cli);
  static GuiTask guiTask = GuiTask("GuiTask", Board::Lcd);
  static ButtonTask buttonTask = ButtonTask("ButtonTask", guiTask);
  Tasks::TaskManager &taskManager = Tasks::TaskManager::Get();
  Board::Init();

  static auto cliKeyHandle = [](uint8_t data)
  {
    buttonTask.cliKeyHandle(data);
  };

  static auto cliKeyEmulate = []()
  {
    cli.setByteHandle(cliKeyHandle);
  };

  cli.Open();
  cli.AddCmd(cmd_t{"key_emulate", NULL, cliKeyEmulate, NULL});
  taskManager.AddTask(cliTask, 4);
  taskManager.AddTask(guiTask, 4);
  taskManager.AddTask(buttonTask, 4);
  taskManager.Start();
  return -1;
}