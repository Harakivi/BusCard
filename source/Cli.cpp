#include "Cli.hpp"

using namespace Drivers;

Cli::Cli(Hardware::iUart &uart) : _uart(uart), _headerUpdater(nullptr), cli_header_len(0),
                                  cli_echo_len(0), needToParse(false), needToUpdateCli(true), _byteHandle(nullptr)
{
}

void Cli::Open(uint32_t BaudRate)
{
    _uart.Open(BaudRate, this);
}

void Cli::setHeaderUpdater(void (*headerUpdater)())
{
    _headerUpdater = headerUpdater;
}

void Cli::Loop(uint32_t time)
{
    static bool errorParse = false;
    static uint32_t startErrorTime = 0;
    static uint32_t lastHeaderUpdateTime = 0;
    if (needToParse)
    {
        print("\033[2K\r");
        if (!parseCmd((char *)cli_echo))
        {
            print("Error \"%s\" not found", (char *)cli_echo);
            errorParse = true;
            startErrorTime = time;
        }
        cli_echo_len = 0;
        cli_echo[cli_echo_len] = 0;
        needToParse = false;
        needToUpdateCli = true;
    }
    if (needToUpdateCli && !errorParse)
    {
        // print("\033c");
        print("\r\n");
        _uart.Write(cli_header, cli_header_len);
        print("\033[0m");
        print("Shell->");
        _uart.Write(cli_echo, cli_echo_len);
        needToUpdateCli = false;
    }
    if (errorParse && (time - startErrorTime) > 1000)
    {
        errorParse = false;
    }
    if (_headerUpdater != nullptr && time - lastHeaderUpdateTime > 1000)
    {
        _headerUpdater();
        lastHeaderUpdateTime = time;
    }
}

void Cli::AddCmd(cmd_t cmd)
{
    addCmd(cmd);
}

int Cli::print(const char *format, ...)
{
    uint32_t writted = 0;
    va_list va;
    va_start(va, format);
    char buffer[256];
    writted = vsnprintf(buffer, 256, format, va);
    va_end(va);
    _uart.Write((uint8_t *)buffer, writted);
    return writted;
}

int Cli::printHeader(const char *format, ...)
{
    uint32_t writted = 0;
    va_list va;
    va_start(va, format);
    writted = vsnprintf((char *)(cli_header + cli_header_len), 256 - cli_header_len, format, va);
    va_end(va);
    cli_header_len += writted;
    needToUpdateCli = true;
    return writted;
}

void Cli::clear()
{
    print("\033c");
}

void Cli::clearHeader()
{
    cli_header_len = 0;
}

void Cli::setByteHandle(void (*byteHandle)(uint8_t data))
{
    _byteHandle = byteHandle;
}

void Cli::onByteReceived(uint8_t data)
{
    if (_byteHandle)
    {
        _byteHandle(data);
        return;
    }
    if (data != '\n' && data != '\r' && data != 127)
    {
        cli_echo[cli_echo_len++] = data;
        if (cli_echo_len < 99)
        {
            cli_echo[cli_echo_len] = 0;
        }
        _uart.Write(&data, 1);
    }
    else if (data == '\r')
    {
        needToParse = true;
    }
    else if (data == 127 && cli_echo_len > 0)
    {
        cli_echo[--cli_echo_len] = 0;
        _uart.Write((uint8_t *)"\b \b", 3);
    }
}