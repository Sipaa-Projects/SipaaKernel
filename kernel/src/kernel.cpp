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

#include <cstdint>

using namespace Sk;
using namespace Sk::Arch;
using namespace Sk::Dev;
using namespace Sk::Disk;
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

char *cat(const char *filename, struct BootSector *_bs) {
    char formatted_name[12];
    DiskUtil::FormatFilename83(filename, formatted_name);

    uint32_t sector = _bs->reserved_sector_count + (_bs->fat_count * _bs->table_size_32);
    struct DirectoryEntry entries[_bs->bytes_per_sector / sizeof(struct DirectoryEntry)];
    bool file_found = false;
    char *file_contents = 0x0;

    for (uint32_t i = 0; i < _bs->sectors_per_cluster && !file_found; i++) {
        DiskUtil::ReadSector(sector + i, entries);

        for (uint32_t j = 0; j < _bs->bytes_per_sector / sizeof(struct DirectoryEntry); j++) {
            if (entries[j].name[0] == 0x00) {
                break;
            } else if ((uint8_t)entries[j].name[0] == 0xE5) {
                continue;
            }

            char entry_name[12];
            BasicMemoryManagement::MemorySet32(entry_name, 0, sizeof(entry_name));
            BasicMemoryManagement::MemoryCopy(entry_name, entries[j].name, 11);

            if (Lib::StringCompare(entry_name, formatted_name) == 0) {
                file_found = true;
                uint32_t cluster = (entries[j].cluster_high << 16) | entries[j].cluster_low;
                uint32_t file_size = entries[j].size;
                char buffer[_bs->bytes_per_sector];

                while (file_size > 0) {
                    DiskUtil::ReadSector(DiskUtil::ClusterToSector(cluster, _bs), buffer);
                    uint32_t size_to_print = (file_size > _bs->bytes_per_sector) ? _bs->bytes_per_sector : file_size;
                    for (uint32_t k = 0; k < size_to_print; k++) {
                        file_contents[k] = buffer[k];
                    }
                    file_size -= size_to_print;

                    uint32_t fat_sector = _bs->reserved_sector_count + (cluster * 4) / _bs->bytes_per_sector;
                    uint32_t fat_offset = (cluster * 4) % _bs->bytes_per_sector;
                    DiskUtil::ReadSector(fat_sector, buffer);
                    cluster = *(uint32_t *)&buffer[fat_offset] & 0x0FFFFFFF;

                    if (cluster == 0x0FFFFFFF) {
                        break;
                    }
                }
                break;
            }
        }
    }

    return file_contents;
}


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
    char *filecontent = cat("file.txt", DiskUtil::BootSector);
    Logger::LogFormatted(LogType_Info, "file.txt content : %s", filecontent);
    Logger::PrintNewLine();

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