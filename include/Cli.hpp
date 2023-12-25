#pragma once
#include "UsbCdc.hpp"
#include <stdarg.h>
#include <stdint.h>
#include "cmd_parser.h"

namespace Drivers
{
    class Cli: public Hardware::UartHandler
    {
    private:
        Hardware::iUart& _uart;
        void(*_headerUpdater)();
        uint8_t cli_header[256];
        uint8_t cli_header_len;
        uint8_t cli_echo[100];
        uint8_t cli_echo_len;
        bool needToParse;
        bool needToUpdateCli;
        void (*_byteHandle)(uint8_t data);
    public:
        Cli(Hardware::iUart& uart);
        void Open(uint32_t BaudRate = 115200);
        void Loop(uint32_t time);
        void AddCmd(cmd_t cmd);
        int print(const char *format, ...);
        void setHeaderUpdater(void(*_headerUpdater)());
        int printHeader(const char *format, ...);
        void clear();
        void clearHeader();
        void setByteHandle(void (*byteHandle)(uint8_t data));
        void onByteReceived(uint8_t data);
    };
}