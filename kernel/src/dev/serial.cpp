#include "serial.h"

#include <arch/io.h>
#include <logging/logger.h>

using namespace Sk::Arch;
using namespace Sk::Logging;

namespace Sk {
namespace Dev {

void Serial::Init()
{
    Logger::Log(LogType_Info, "Initializing serial console...");
    Io::OutB(SERIAL_PORT + 1, 0x00);
    Io::OutB(SERIAL_PORT + 3, 0x80);
    Io::OutB(SERIAL_PORT + 0, 0x01);
    Io::OutB(SERIAL_PORT + 1, 0x00);
    Io::OutB(SERIAL_PORT + 3, 0x03);
    Io::OutB(SERIAL_PORT + 2, 0xC7);
    Io::OutB(SERIAL_PORT + 4, 0x0B);
    Logger::PrintOK();
}

bool Serial::IsReady()
{
    return Io::InB(SERIAL_PORT + 5) & 0x20;
}

void Serial::SetColor(char *color)
{
    // TODO : Use a custom type for serial colors
    WriteStr(color);
}

void Serial::WriteChar(char ch)
{
    while (!IsReady())
        ;
    
    Io::OutB(SERIAL_PORT, ch);
}

void Serial::WriteStr(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        WriteChar(str[i]);
}

} // namespace Dev
} // namespace Sk