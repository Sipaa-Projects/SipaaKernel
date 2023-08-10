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
        Serial::WriteChar('\n');
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

void Logger::Log(LogType logtype, char *message)
{
    switch (logtype)
    {
    case LogType_Error:
        Serial::SetColor((char *)SERIAL_COLOR_RED);
        Serial::WriteStr("[ERROR] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0xC0392B;
            Console::Print("[ERROR] ");
            Console::term_color = 0xFFFFFF;
        }
        break;
    case LogType_Warning:
        Serial::SetColor((char *)SERIAL_COLOR_YELLOW);
        Serial::WriteStr("[WARN] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0xF39C12;
            Console::Print("[WARN] ");
            Console::term_color = 0xFFFFFF;
        }
        break;
    case LogType_Info:
        Serial::SetColor((char *)SERIAL_COLOR_BLUE);
        Serial::WriteStr("[INFO] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0x27AE60;
            Console::Print("[INFO] ");
            Console::term_color = 0xFFFFFF;
        }
        break;
    case LogType_Debug:
        Serial::SetColor((char *)SERIAL_COLOR_GREEN);
        Serial::WriteStr("[DEBUG] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0x2980B9;
            Console::Print("[DEBUG] ");
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

void Logger::LogFormatted(LogType logtype, char *message, ...)
{
    switch (logtype)
    {
    case LogType_Error:
        Serial::SetColor((char *)SERIAL_COLOR_RED);
        Serial::WriteStr("[ERROR] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0xC0392B;
            Console::Print("[ERROR] ");
            Console::term_color = 0xFFFFFF;
        }
        break;
    case LogType_Warning:
        Serial::SetColor((char *)SERIAL_COLOR_YELLOW);
        Serial::WriteStr("[WARN] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0xF39C12;
            Console::Print("[WARN] ");
            Console::term_color = 0xFFFFFF;
        }
        break;
    case LogType_Info:
        Serial::SetColor((char *)SERIAL_COLOR_BLUE);
        Serial::WriteStr("[INFO] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0x27AE60;
            Console::Print("[INFO] ");
            Console::term_color = 0xFFFFFF;
        }
        break;
    case LogType_Debug:
        Serial::SetColor((char *)SERIAL_COLOR_GREEN);
        Serial::WriteStr("[DEBUG] ");
        Serial::SetColor((char *)SERIAL_COLOR_WHITE);

        if (!Global::Framebuffer.UseDoubleBuffer)
        { 
            Console::term_color = 0x2980B9;
            Console::Print("[DEBUG] ");
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