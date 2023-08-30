#include "global.h"

#include <arch/gdt.h>
#include <arch/idt.h>
#include <dev/serial.h>
#include <console/console.h>
#include <lib/lib.h>
#include <limine/limine.h>
#include <logging/logger.h>
#include <memory/memory.h>
#include <system/system.h>
#include <video/video.h>

#include <stdint.h>

using namespace Sk;
using namespace Sk::Arch;
using namespace Sk::Dev;
using namespace Sk::Graphic;
using namespace Sk::Logging;
using namespace Sk::Memory;

uint64_t kernel_stack[8192];

static volatile struct limine_framebuffer_request fbr = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

void SK_ShowBootInfo(char *kepfname)
{
    Logger::Log(LogType_Info, "SK", "Bootloader : Limine\n");

    #if defined(__aarch64__)
    Logger::Log(LogType_Info, "SK", "Architecture : 64bits ARM (aarch64)\n");
    #elif defined(__riscv64__)
    Logger::Log(LogType_Info, "SK", "Architecture : 64bits RISC-V (riscv64)\n");
    #elif defined(__x86_64__)
    Logger::Log(LogType_Info, "SK", "Architecture : 64bits x86 (x86-64)\n");
    #endif

    Logger::LogFormatted(LogType_Info, "SK", "Framebuffer : %llxx%llxx%d\n", fbr.response->framebuffers[0]->width, fbr.response->framebuffers[0]->height, fbr.response->framebuffers[0]->bpp);
    Logger::Log(LogType_Info, "SK", "Kernel Entry Point : ");
    Logger::PrintNoStart(kepfname);
    Logger::PrintNoStart(" (defined in ");
    Logger::PrintNoStart(__FILE__);
    Logger::PrintNoStart(")\n");
}

void memtest()
{
    void *block1 = PhysicalMemoryManager::MAlloc(4096);
    Logger::LogFormatted(LogType_Debug, "MemTest", "Allocated block at: 0x%llx with size : %d\n", (uint64_t)block1, 4096);

    PhysicalMemoryManager::Free(block1);
    Logger::LogFormatted(LogType_Debug, "MemTest", "Freed block at: 0x%llx\n", (uint64_t)block1);

    void *block2 = PhysicalMemoryManager::MAlloc(4096);
    Logger::LogFormatted(LogType_Debug, "MemTest", "Allocated block at: 0x%llx with size : %d\n", (uint64_t)block2, 4096);

    if (block1 == block2)
    {
        Logger::Log(LogType_Debug, "MemTest", "The allocation after free gave us the same block. Dynamic memory management seems to work!\n");
    }
    else
    {
        Logger::Log(LogType_Debug, "MemTest", "Hmm, the blocks are different. Something might be off.\n");
    }
}

extern "C" void SK_Main()
{
    Framebuffer f = Framebuffer::FromLimine(fbr.response->framebuffers[0]);
    f.UseDoubleBuffer = false;

    Global::Framebuffer = f;

    Console::Init(f);
    Serial::Init();

    Logger::Log(LogType_Info, "SK", "Welcome to SipaaKernel!\n");
    SK_ShowBootInfo(__FUNCTION__);

    #if defined(__x86_64__)
        Gdt::Init((uint64_t)&kernel_stack + sizeof(kernel_stack));
        Idt::Init();
    #endif

    PhysicalMemoryManager::Init();
    memtest();

    f.UseDoubleBuffer = true;

    System::panic("End of kernel entry point function has been reached.");
}