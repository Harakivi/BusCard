#include "Board.hpp"
#include "TaskManager.hpp"
#include "Cli.hpp"
#include "Tasks/CliTask.hpp"
#include "Tasks/ButtonsTask.hpp"
#include "Tasks/GuiTask.hpp"

typedef Hardware::BusCard Board;

typedef Tasks::CliTask<256 * 4> CliTask;
typedef Tasks::ButtonTask<256 * 6> ButtonTask;
typedef Tasks::GuiTask<256 * 6> GuiTask;

int main()
{
  Board::Init();
  static Drivers::Cli cli = Drivers::Cli(Board::cliUart);
  static CliTask cliTask = CliTask("CliTask", cli);
  static GuiTask guiTask = GuiTask("GuiTask", Board::Lcd);
  static std::initializer_list<Drivers::iButtons *> buttonSubscribers{&guiTask};
  static ButtonTask buttonTask = ButtonTask("ButtonTask", buttonSubscribers);

  Tasks::TaskManager &taskManager = Tasks::TaskManager::Get();

  static auto cliKeyHandle = [](uint8_t data)
  {
    buttonTask.cliKeyHandle(data);
  };

  static auto cliKeyEmulate = []()
  {
    cli.setByteHandle(cliKeyHandle);
  };

  cli.AddCmd(Drivers::cmd_t{"key_emulate", NULL, cliKeyEmulate, NULL});
  cli.Open();
  taskManager.AddTask(cliTask, 4);
  taskManager.AddTask(guiTask, 4);
  taskManager.AddTask(buttonTask, 4);
  taskManager.Start();
}