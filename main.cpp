#include "Board.hpp"
#include "TaskManager.hpp"
#include "Cli.hpp"
#include "Tasks/CliTask.hpp"
#include "Tasks/ButtonsTask.hpp"
#include "Tasks/GuiTask.hpp"

typedef Hardware::BusCard Board;

typedef Tasks::ButtonTask<256 * 10> ButtonTask;
typedef Tasks::GuiTask<256 * 20> GuiTask;

int main()
{
  Board::Init();
  Board::DispBlk::Get().Init(Hardware::iGpio::GPIO_Modes::OutputPushPull);
  Board::DispBlk::Get().SetVal(false);
  Board::SdioEn::Get().Init(Hardware::iGpio::GPIO_Modes::OutputPushPull);
  Board::SdioEn::Get().SetVal(false);
  
  static GuiTask guiTask = GuiTask("GuiTask", Board::Lcd);
  static std::initializer_list<Drivers::iButtons *> buttonSubscribers{&guiTask};
  static ButtonTask buttonTask = ButtonTask("ButtonTask", buttonSubscribers);

  Tasks::TaskManager &taskManager = Tasks::TaskManager::Get();

  taskManager.AddTask(guiTask, 4);
  taskManager.AddTask(buttonTask, 4);
  taskManager.Start();
}