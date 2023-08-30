#include "serial.h"

#include <arch/io.h>
#include <logging/logger.h>
#include <lib/lib.h>

//#define SERIAL_DISABLED // You can uncomment this so you will have the serial console disabled, then faster performance. Will be disabled in each SipaaKernel release.

using namespace Sk::Arch;
using namespace Sk::Logging;

namespace Sk {
namespace Dev {

void Serial::Init()
{
    #ifndef __aarch64__
    #ifndef SERIAL_DISABLED
    Io::OutB(SERIAL_PORT + 1, 0x00);
    Io::OutB(SERIAL_PORT + 3, 0x80);
    Io::OutB(SERIAL_PORT + 0, 0x01);
    Io::OutB(SERIAL_PORT + 1, 0x00);
    Io::OutB(SERIAL_PORT + 3, 0x03);
    Io::OutB(SERIAL_PORT + 2, 0xC7);
    Io::OutB(SERIAL_PORT + 4, 0x0B);
    #endif
    #endif
}

bool Serial::IsReady()
{
    #ifndef __aarch64__
    #ifndef SERIAL_DISABLED
    return Io::InB(SERIAL_PORT + 5) & 0x20;
    #else
    return true;
    #endif
    return true;
    #endif
}

void Serial::SetColor(char *color)
{
    // TODO : Use a custom type for serial colors
    WriteStr(color);
}

void Serial::WriteChar(char ch)
{
    #ifndef __aarch64__
    #ifndef SERIAL_DISABLED
    while (!IsReady())
        ;
    
    Io::OutB(SERIAL_PORT, ch);
    #endif
    #endif
}

void Serial::WriteStr(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        WriteChar(str[i]);
}


void Serial::WriteInt(int v, int base, const char *digits)
{
    char buf[33];
    char *ptr = &buf[sizeof(buf) - 1];
    *ptr = '\0';

    if (v == 0)
    {
        WriteChar('0');
        return;
    }

    if (v < 0 && base == 10)
    {
        WriteChar('-');
        v = -v;
    }

    while (v)
    {
        *--ptr = digits[v % base];
        v /= base;
    }

    WriteStr(ptr);
}

void Serial::WriteInt64(uint64_t v, int base, const char *digits)
{
    char buf[65];
    char *pointer = &buf[sizeof(buf) - 1];
    *pointer = '\0';

    if (v == 0)
    {
        WriteChar('0');
        return;
    }

    while (v)
    {
        *--pointer = digits[v % base];
        v /= base;
    }

    WriteStr(pointer);
}

void Serial::WriteStrFormatted(char *format, ...)
{
    va_list args;
    va_start(args, format);

    const char *hex_digits = "0123456789ABCDEF";

    while (*format != '\0')
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 's':
            {
                char *str = va_arg(args, char *);
                WriteStr(str);
                break;
            }
            case 'c':
            {
                char c = (char)va_arg(args, int);
                WriteChar(c);
                break;
            }
            case 'd':
            {
                int d = va_arg(args, int);
                WriteInt(d, 10, hex_digits);
                break;
            }
            case 'x':
            {
                int x = va_arg(args, int);
                WriteInt(x, 16, hex_digits);
                break;
            }
            case 'l':
            {
                format++;
                if (*format == 'l' && *(format + 1) == 'x')
                {
                    format++;
                    uint64_t llx = va_arg(args, uint64_t);
                    WriteInt64(llx, 16, hex_digits);
                }
                else
                {
                    WriteStr("Invalid format specifier");
                }
                break;
            }
            default:
                WriteChar('%');
                WriteChar(*format);
                break;
            }
        }
        else
        {
            WriteChar(*format);
        }
        format++;
    }

    va_end(args);
}

} // namespace Dev
} // namespace Sk