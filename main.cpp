#include "Board.hpp"
#include "Interfaces/iTask.hpp"
#include "Interfaces/iLcd.hpp"
#include "TaskManager.hpp"
#include "Cli.hpp"
#include "CliTask.hpp"

typedef Tasks::CliTask<256 * 4> CliTask;
typedef Hardware::BusCard Board;
namespace Tasks
{
  template <uint32_t StackSize>
  class LcdTask : public iTask
  {
    uint8_t taskStack[StackSize];
    TaskManager &_taskManager;
    Drivers::iLcd &_lcd;

  public:
    LcdTask(const char *name, Drivers::iLcd &lcd) : iTask{name}, _taskManager(TaskManager::Get()), _lcd(lcd) {}

    virtual void Task()
    {
      uint8_t r = 0, g = 0, b = 0;
      uint16_t x = 0;
      uint16_t y = 0;
      _lcd.SetFont(_lcd.Font_16x26);
      for (;;)
      {
        r++;
        g++;
        b++;
        _lcd.FillColor(_lcd.GetColorFromRGB(r, g, b));
        _lcd.WriteString("Hello World", x++, y++, _lcd.Yellow565, _lcd.Blue565);
        if (x > _lcd.Width())
          x = 0;
        if (y > _lcd.Height())
          y = 0;
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
  };
}

typedef Tasks::LcdTask<256 * 4> LcdTask;

int main()
{
  static Drivers::Cli cli = Drivers::Cli(Board::cliUart);
  static CliTask cliTask = CliTask("CliTask", cli);
  static LcdTask lcdTask = LcdTask("LcdTask", Board::Lcd);
  Tasks::TaskManager &taskManager = Tasks::TaskManager::Get();
  Board::Init();

  cli.Open();

  taskManager.AddTask(cliTask, 4);
  taskManager.AddTask(lcdTask, 4);
  taskManager.Start();
  return -1;
}