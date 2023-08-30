#include "logger.h"

#include <dev/serial.h>
#include <console/console.h>
#include <lib/lib.h>
#include <global.h>

using namespace Sk::Dev;

namespace Sk {
namespace Logging {

void Logger::PrintNewLine()
{
    Serial::WriteChar('\n');

    if (!Global::Framebuffer.UseDoubleBuffer)
        Console::PrintChar('\n');
}

void Logger::PrintNoStart(char *str)
{
    Serial::WriteStr(str);

    if (!Global::Framebuffer.UseDoubleBuffer)
        Console::Print(str);
}

void Logger::PrintOK()
{
    Serial::SetColor((char*)SERIAL_COLOR_GREEN);
    Serial::WriteStr(" OK");
    Serial::SetColor((char*)SERIAL_COLOR_WHITE);
    Serial::WriteChar('\n');

    if (!Global::Framebuffer.UseDoubleBuffer)
    {
        Console::term_color = 0x2980B9;
        Console::Print(" OK");
        Console::term_color = 0xFFFFFF;
        Console::PrintChar('\n');
    }
}

void Logger::PrintFail()
{
    Serial::SetColor((char*)SERIAL_COLOR_RED);
    Serial::WriteStr(" FAIL");
    Serial::SetColor((char*)SERIAL_COLOR_WHITE);
    Serial::WriteChar('\n');

    if (!Global::Framebuffer.UseDoubleBuffer)
    {
        Console::term_color = 0xC0392B;
        Console::Print(" OK");
        Console::term_color = 0xFFFFFF;
        Console::PrintChar('\n');
    }
}

void Logger::Log(LogType logtype, char *comp, char *message)
{
    switch (logtype)
    {
    case LogType_Error:
        Serial::SetColor((char *)SERIAL_COLOR_RED);
        Serial::WriteChar('[');
        Serial::WriteStr(comp);
        Serial::WriteStr(" / ERROR] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0xC0392B;
            Console::PrintChar('[');
            Console::Print(comp);
            Console::Print(" / ERROR] ");
            Console::term_color = 0xFFFFFF;
        }
        break;
    case LogType_Warning:
        Serial::SetColor((char *)SERIAL_COLOR_YELLOW);
        Serial::WriteChar('[');
        Serial::WriteStr(comp);
        Serial::WriteStr(" / WARN] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0xF39C12;
            Console::PrintChar('[');
            Console::Print(comp);
            Console::Print(" / WARN] ");
            Console::term_color = 0xFFFFFF;
        }
        break;
    case LogType_Info:
        Serial::SetColor((char *)SERIAL_COLOR_BLUE);
        Serial::WriteChar('[');
        Serial::WriteStr(comp);
        Serial::WriteStr(" / INFO] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0x27AE60;
            Console::PrintChar('[');
            Console::Print(comp);
            Console::Print(" / INFO] ");
            Console::term_color = 0xFFFFFF;
        }
        break;
    case LogType_Debug:
        Serial::SetColor((char *)SERIAL_COLOR_GREEN);
        Serial::WriteChar('[');
        Serial::WriteStr(comp);
        Serial::WriteStr(" / DEBUG] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0x2980B9;
            Console::PrintChar('[');
            Console::Print(comp);
            Console::Print(" / DEBUG] ");
            Console::term_color = 0xFFFFFF;
        }
        break;
    
    default:
        Serial::WriteStr("[UNKNOWN] ");
        if (!Global::Framebuffer.UseDoubleBuffer)
            Console::Print("[UNKNOWN] ");
        break;
    }

    Serial::WriteStr(message);
    if (!Global::Framebuffer.UseDoubleBuffer)
        Console::Print(message);
}

void Logger::LogFormatted(LogType logtype, char *comp, char *message, ...)
{
    switch (logtype)
    {
    case LogType_Error:
        Serial::SetColor((char *)SERIAL_COLOR_RED);
        Serial::WriteChar('[');
        Serial::WriteStr(comp);
        Serial::WriteStr(" / ERROR] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0xC0392B;
            Console::PrintChar('[');
            Console::Print(comp);
            Console::Print(" / ERROR] ");
            Console::term_color = 0xFFFFFF;
        }
        break;
    case LogType_Warning:
        Serial::SetColor((char *)SERIAL_COLOR_YELLOW);
        Serial::WriteChar('[');
        Serial::WriteStr(comp);
        Serial::WriteStr(" / WARN] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0xF39C12;
            Console::PrintChar('[');
            Console::Print(comp);
            Console::Print(" / WARN] ");
            Console::term_color = 0xFFFFFF;
        }
        break;
    case LogType_Info:
        Serial::SetColor((char *)SERIAL_COLOR_BLUE);
        Serial::WriteChar('[');
        Serial::WriteStr(comp);
        Serial::WriteStr(" / INFO] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0x27AE60;
            Console::PrintChar('[');
            Console::Print(comp);
            Console::Print(" / INFO] ");
            Console::term_color = 0xFFFFFF;
        }
        break;
    case LogType_Debug:
        Serial::SetColor((char *)SERIAL_COLOR_GREEN);
        Serial::WriteChar('[');
        Serial::WriteStr(comp);
        Serial::WriteStr(" / DEBUG] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0x2980B9;
            Console::PrintChar('[');
            Console::Print(comp);
            Console::Print(" / DEBUG] ");
            Console::term_color = 0xFFFFFF;
        }
        break;
    
    default:
        Serial::WriteStr("[UNKNOWN] ");
        if (!Global::Framebuffer.UseDoubleBuffer)
            Console::Print("[UNKNOWN] ");
        break;
    }

    va_list args;
    va_start(args, message);

    Serial::WriteStrFormatted(message, args);
    if (!Global::Framebuffer.UseDoubleBuffer)
        Console::PrintFormatted(message, args);

    va_end(args);
}

} // namespace Dev
} // namespace Sk