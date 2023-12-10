#include "Board.hpp"
#include "usbcdc.hpp"
#include "Cli.hpp"

typedef InternalPeriph::UsbCdc cliUart;

typedef Drivers::Cli Cli;
Cli cli = Cli(cliUart::Get());

int main()
{
  BoardInit();
  cli.Open();
  
  while(true)
  {
    cli.Loop(HAL_GetTick());
  }
}
