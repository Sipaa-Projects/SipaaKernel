#include "system.h"
#include <logging/logger.h>
#include <console/console.h>
#include <global.h>

using namespace Sk::Logging;

namespace Sk
{

void System::_panic(char *msg, char *file, char *line, struct Registers64 *regs)
{
    Global::Framebuffer.UseDoubleBuffer = false;
    Sk::Console::Reset();

    Logger::LogFormatted(LogType_Error, "Kernel Panic :'(\n");
    Logger::LogFormatted(LogType_Error, "We are sorry than this error happened to you\n");
    Logger::PrintNewLine();
    Logger::LogFormatted(LogType_Error, "%s: %s: %s\n", file, line, msg);
    Logger::PrintNewLine();
    Logger::LogFormatted(LogType_Error, "If you are running SipaaKernel in QEMU, you can do 'info registers'\n");
    Logger::LogFormatted(LogType_Error, "in the compat monitor to get CPU registers.\n");
    Logger::PrintNewLine();
    Logger::LogFormatted(LogType_Error, "System halted.\n");

    while (1)
        asm("hlt");
}

}
