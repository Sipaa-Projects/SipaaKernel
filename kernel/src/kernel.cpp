#include "global.h"

#include <arch/gdt.h>
#include <arch/idt.h>
#include <dev/serial.h>
#include <dev/pci.h>
#include <dev/ps2.h>
#include <disk/disk.h>
#include <console/console.h>
#include <lib/lib.h>
#include <limine/limine.h>
#include <logging/logger.h>
#include <memory/memory.h>
#include <video/video.h>

#include <stdint.h>

using namespace Sk;
using namespace Sk::Arch;
using namespace Sk::Dev;
using namespace Sk::Disk;
using namespace Sk::Graphic;
using namespace Sk::Logging;
using namespace Sk::Memory;

uint64_t kernel_stack[8192];

typedef struct {
    int x;
    int y;
    int speed;
} Star;

void moveStar(Star *star, uint32_t screenWidth, uint32_t screenHeight) {
    star->x -= star->speed;
    if (star->x <= 0) {
        star->x = screenWidth;
        star->y = Lib::Random() % screenWidth;
        star->speed = Lib::Random() % 3 + 1;
    }
}

static volatile struct limine_framebuffer_request fbr = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

#ifdef __x86_64__
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
#endif

void SK_ShowBootInfo()
{
    Logger::Log(LogType_Debug, "Bootloader : Limine\n");

    #if defined(__aarch64__)
    Logger::Log(LogType_Debug, "Architecture : 64bits ARM (aarch64)\n");
    #elif defined(__riscv64__)
    Logger::Log(LogType_Debug, "Architecture : 64bits RISC-V (riscv64)\n");
    #elif defined(__x86_64__)
    Logger::Log(LogType_Debug, "Architecture : 64bits x86 (x86-64)\n");
    #endif

    Logger::LogFormatted(LogType_Debug, "Framebuffer : %dx%dx%d\n", fbr.response->framebuffers[0]->width, fbr.response->framebuffers[0]->height, fbr.response->framebuffers[0]->bpp);
}

// User-space function
extern "C" void usr_main() {
    Logger::Log(LogType_Debug, "Welcome to userspace!");
    while (1) {
        // Your user-space code here
    }
}

extern "C" void jump_usermode();

extern "C" void SK_Main()
{
    Framebuffer f = Framebuffer::FromLimine(fbr.response->framebuffers[0]);
    f.UseDoubleBuffer = false;

    Global::Framebuffer = f;

    Console::Init(f);
    Serial::Init();

    Logger::Log(LogType_Info, "Welcome to SipaaKernel!\n");
    SK_ShowBootInfo();

    #if defined(__x86_64__)
        Gdt::Init((uint64_t)&kernel_stack + sizeof(kernel_stack));
        Idt::Init();
        MemoryAllocator::Init(memmap_request.response->entries, memmap_request.response->entry_count);
        MemoryAllocator::InitVMM(memmap_request.response->entries, memmap_request.response->entry_count);
        PS2::Init();

        if (DiskUtil::GetDiskFormat(0) == FAT32)
        {
            Logger::Log(LogType_Debug, "FAT32-formatted disk detected\n");
        }
        DiskUtil::BootSector = MemoryAllocator::Allocate(sizeof(struct BootSector));
        if (DiskUtil::BootSector == NULL)
        {
            Logger::Log(LogType_Error, "Failed to allocate memory for boot sector.\n");
            while (1)
                ;
        }
        DiskUtil::ReadSector(0, DiskUtil::BootSector);
        DiskUtil::CurrentDirectoryCluster = DiskUtil::BootSector->root_cluster;

        memory_test();
    #else

    #endif

    f.UseDoubleBuffer = true;

    jump_usermode();

    Lib::InitRNG(0);
    Star stars[256];

    for (int i = 0; i < 256; i++) {
        stars[i].x = Lib::Random() % f.Width;
        stars[i].y = Lib::Random() % f.Height;
        stars[i].speed = Lib::Random() % 3 + 1;
    }   

    while (1)
    {
        f.Clear(0x0);

        for (int i = 0; i < 256; i++) {
            f.SetPixel(stars[i].x, stars[i].y, 0xFFFFFFFF); // White color
            moveStar(&stars[i], f.Width, f.Height);
        }

        f.SwapBuffers();
    }
}