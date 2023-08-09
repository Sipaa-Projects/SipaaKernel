#include "serial.h"

#include <arch/io.h>
#include <logging/logger.h>
#include <lib/lib.h>

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

void Serial::WriteStrFormatted(char *str, ...)
{
    va_list args;
    va_start(args, str);

    char buffer[256];

    for (const char *p = str; *p != '\0'; p++)
    {
        if (*p != '%')
        {
            WriteChar(*p);
            continue;
        }

        switch (*++p)
        {
        case 'c':
            WriteChar(va_arg(args, int));
            break;
        case 'd':
            Lib::IToString(va_arg(args, int), buffer, 10);
            for (char *p = buffer; *p != '\0'; p++)
                WriteChar(*p);
            break;
        case 'u':
            Lib::UToString(va_arg(args, unsigned int), buffer, 10);
            for (char *p = buffer; *p != '\0'; p++)
                WriteChar(*p);
            break;
        case 'x':
            Lib::UToString(va_arg(args, unsigned int), buffer, 16);
            for (char *p = buffer; *p != '\0'; p++)
                WriteChar(*p);
            break;
        case 's':
            for (char *p = va_arg(args, char *); *p != '\0'; p++)
                WriteChar(*p);
            break;
        case 'l':
            switch (*++p)
            {
            case 'l':
                switch (*++p)
                {
                case 'u':
                    Lib::UlliToString(va_arg(args, unsigned long long int), buffer, 10);
                    for (char *p = buffer; *p != '\0'; p++)
                        WriteChar(*p);
                    break;
                case 'x':
                    Lib::UlliToString(va_arg(args, unsigned long long int), buffer, 16);
                    for (char *p = buffer; *p != '\0'; p++)
                        WriteChar(*p);
                    break;
                }
                break;
            case 'd':
                Lib::IToString(va_arg(args, long int), buffer, 10);
                for (char *p = buffer; *p != '\0'; p++)
                    WriteChar(*p);
                break;
            case 'u':
                Lib::UToString(va_arg(args, unsigned long int), buffer, 10);
                for (char *p = buffer; *p != '\0'; p++)
                    WriteChar(*p);
                break;
            case 'x':
                Lib::UToString(va_arg(args, unsigned long int), buffer, 16);
                for (char *p = buffer; *p != '\0'; p++)
                    WriteChar(*p);
                break;
            }
            break;

        case 'p':
            Lib::UToString(va_arg(args, uintptr_t), buffer, 16);
            for (char *p = buffer; *p != '\0'; p++)
                WriteChar(*p);
            break;
        }
    }

    va_end(args);
}

} // namespace Dev
} // namespace Sk