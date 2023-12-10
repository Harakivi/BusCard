#include "Board.hpp"
#include "TaskManager.hpp"
#include "usbcdc.hpp"
#include "Cli.hpp"
#include "CliTask.hpp"

typedef InternalPeriph::UsbCdc cliUart;
typedef Tasks::CliTask<256> CliTask;

int main()
{
  static Drivers::Cli cli = Drivers::Cli(cliUart::Get());
  static CliTask cliTaskInstance = CliTask(&cli);

  BoardInit();
  cli.Open();

  TaskManager::Get()->AddTask(&cliTaskInstance, configMAX_PRIORITIES - 1);
  TaskManager::Get()->Start();
  return -1;
}