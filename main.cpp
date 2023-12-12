#include "Board.hpp"
#include "Interfaces.hpp"
#include "Cli.hpp"
#include "CliTask.hpp"

typedef Tasks::CliTask<256 * 4> CliTask;
typedef Hardware::BusCard Board;

int main()
{
  static Drivers::Cli cli = Drivers::Cli(Board::cliUart::Get());
  static CliTask cliTask = CliTask("CliTask", cli);
  Tasks::TaskManager& taskManager = Tasks::TaskManager::Get();
  Board::Init();

  cli.Open();

  taskManager.AddTask(cliTask, 4);
  taskManager.Start();
  return -1;
}