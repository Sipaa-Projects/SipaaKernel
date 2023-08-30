#include "system.h"
#include <logging/logger.h>
#include <console/console.h>
#include <global.h>

using namespace Sk::Logging;

namespace Sk
{

void System::_panic(char *msg, char *file, char *line)
{
    Global::Framebuffer.UseDoubleBuffer = false;
    Sk::Console::Reset();

    Logger::Log(LogType_Error, "SK / Panic", "Kernel Panic :'(\n");
    Logger::Log(LogType_Error, "SK / Panic", "We are sorry than this error happened to you\n");
    Logger::Log(LogType_Error, "SK / Panic", "\n");
    Logger::Log(LogType_Error, "SK / Panic", file);
    Logger::PrintNoStart(" : ");
    Logger::PrintNoStart(msg);
    Logger::PrintNewLine();
    Logger::Log(LogType_Error, "SK / Panic", "\n");
    Logger::Log(LogType_Error, "SK / Panic", "If you are running SipaaKernel in QEMU, you can do 'info registers'\n");
    Logger::Log(LogType_Error, "SK / Panic", "in the compat monitor (CTRL+ALT+2) to get CPU registers.\n");
    Logger::Log(LogType_Error, "SK / Panic", "\n");
    Logger::Log(LogType_Error, "SK / Panic", "System halted.\n");

    while (1)
    {
        #if defined(__aarch64__) 
        asm("wfi");
        #elif defined(__x86_64__)
        asm("hlt");
        #endif
    }
}

}