#include "global.h"

#include <arch/gdt.h>
#include <arch/idt.h>
#include <dev/serial.h>
#include <dev/pci.h>
#include <dev/ps2.h>
#include <console/console.h>
#include <lib/lib.h>
#include <limine/limine.h>
#include <logging/logger.h>
#include <memory/memory.h>
#include <video/video.h>

#include <cstdint>

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

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

void memory_test()
{
    Logger::Log(LogType_Info, "Starting memory test...\n");

    Logger::Log(LogType_Info, "Allocating memory...\n");
    char *test_block1 = (char *)MemoryAllocator::Allocate(1000);
    char *test_block2 = (char *)MemoryAllocator::Allocate(2000);

    if (test_block1)
    {
        Logger::Log(LogType_Info, "Writing to allocated memory block 1...\n");
        Lib::CopyString(test_block1, "Hello, World!");
        Logger::Log(LogType_Info, "Reading from allocated memory block 1\n");
    }
    else
    {
        Logger::Log(LogType_Error, "Failed to allocate memory block 1.\n");
    }

    if (test_block2)
    {
        Logger::Log(LogType_Info, "Writing to allocated memory block 2...\n");
        Lib::CopyString(test_block2, "Testing 123...");
        Logger::Log(LogType_Info, "Reading from allocated memory block 2\n");
    }
    else
    {
        Logger::Log(LogType_Error, "Failed to allocate memory block 2.\n");
    }

    Logger::Log(LogType_Info, "Freeing allocated memory...\n");
    if (test_block1)
    {
        MemoryAllocator::Free(test_block1);
    }
    if (test_block2)
    {
        MemoryAllocator::Free(test_block2);
    }

    Logger::Log(LogType_Info, "Memory test completed.\n");
}

extern "C" void SK_Main()
{
    Framebuffer f = Framebuffer::FromLimine(fbr.response->framebuffers[0]);
    f.UseDoubleBuffer = false;

    Global::Framebuffer = f;

    Console::Init(f);
    Serial::Init();
    Gdt::Init((uint64_t)&kernel_stack + sizeof(kernel_stack));
    Idt::Init();
    MemoryAllocator::Init(memmap_request.response->entries, memmap_request.response->entry_count);
    MemoryAllocator::InitVMM(memmap_request.response->entries, memmap_request.response->entry_count);
    PS2::Init();
    memory_test();
    
    //asm("int $0x14");

    f.UseDoubleBuffer = true;
    int x = 0;
    int y = 0;
    bool ttb = true;
    bool ltr = true;

    while (1)
    {
        f.Clear(0x0);

        if (x == f.Width - 100)
            ltr = false;
        if (x == 0)
            ltr = true;

        if (y == f.Height - 100)
            ttb = false;
        if (y == 0)
            ttb = true;

        if (ltr)
            x++;
        else
            x--;

        if (ttb)
            y++;
        else
            y--;

        for (int j = y; j < y + 100; j++)
        {
            for (int i = x; i < x + 100; i++)
            {
                f.SetPixel(i, j, 0xFFFFFF);
            }
        }

        f.SetPixel(PS2::CurrentMouseStatus.X, PS2::CurrentMouseStatus.Y, 0xFFFFFF);
        f.SwapBuffers();
    }
}