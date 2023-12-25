#include "Board.hpp"
#include "iTask.hpp"
#include "iLcd.hpp"
#include "TaskManager.hpp"
#include "Cli.hpp"
#include "CliTask.hpp"
#include "Gui/Button.hpp"
#include "BoardButtons.hpp"
#include "nes.hpp"
#include "rom.h"

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
    Gui::Button btn1 = Gui::Button("nes_emulator");
    Gui::Button btn2 = Gui::Button("TestBtn2");
    Gui::Button btn3 = Gui::Button("TestBtn3");
    Gui::Button btn4 = Gui::Button("TestBtn4");
    NES_EMU nes = NES_EMU((NES_EMU::NesRom *)MarioRomFile);
    Gui::Control *activeControl = &btn1;

  public:
    GuiTask(const char *name, Utils::GFX &lcd) : iTask{name}, _taskManager(TaskManager::Get()), _lcd(lcd) {}

    virtual void Task()
    {
      _lcd.FillColor(_lcd.Black565);
      btn1.view = Gui::View{0, 0, 150, 20, _lcd.Font_11x18};
      btn2.view = Gui::View{0, 25, 100, 20, _lcd.Font_11x18};
      btn3.view = Gui::View{0, 50, 100, 20, _lcd.Font_11x18};
      btn4.view = Gui::View{0, 75, 100, 20, _lcd.Font_11x18};
      btn1.navigation.ToDown = &btn2;
      btn2.navigation.ToDown = &btn3;
      btn3.navigation.ToDown = &btn4;
      btn4.navigation.ToDown = &btn1;
      btn1.navigation.ToUp = &btn4;
      btn2.navigation.ToUp = &btn1;
      btn3.navigation.ToUp = &btn2;
      btn4.navigation.ToUp = &btn3;

      btn1.navigation.Enter = &nes;
      btn1.navigation.Active = true;
      btn1.Draw(_lcd);
      btn2.Draw(_lcd);
      btn3.Draw(_lcd);
      btn4.Draw(_lcd);
      for (;;)
      {
        activeControl->Processing(_lcd);
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
        _taskManager.Delay(50);
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