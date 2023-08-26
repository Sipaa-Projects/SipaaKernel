#include "memory.h"
#include <stddef.h>
#include <logging/logger.h>

using namespace Sk::Logging;

namespace Sk {
namespace Memory {

int BasicMemoryManagement::MemoryCompare(const void *s1, const void *s2, size_t n)
{
    const unsigned char *p1 = s1, *p2 = s2;
    while (n--)
    {
        if (*p1 != *p2)
        {
            return *p1 - *p2;
        }
        p1++;
        p2++;
    }
    return 0;
}

void BasicMemoryManagement::MemorySet(void* start, uint8_t value, size_t size) {
    for (uint64_t i = 0; i < size; i += sizeof(uint8_t)){
        *(uint8_t*)((uint64_t)start + i) = value;
    }
}       

void BasicMemoryManagement::MemorySet16(void* start, uint16_t value, size_t size) {
    for (uint64_t i = 0; i < size; i += sizeof(uint16_t)){
        *(uint16_t*)((uint64_t)start + i) = value;
    }
}

void BasicMemoryManagement::MemorySet32(void* start, uint32_t value, size_t size) {
    for (uint64_t i = 0; i < size; i += sizeof(uint32_t)){
        *(uint32_t*)((uint64_t)start + i) = value;
    }
}

void BasicMemoryManagement::MemorySet64(void* start, uint64_t value, size_t size) {
    for (uint64_t i = 0; i < size; i += sizeof(uint64_t)){
        *(uint64_t*)((uint64_t)start + i) = value;
    }
}

void BasicMemoryManagement::MemoryCopy(void* Destination, void* Source, size_t Size)
{
    char *dest_c = (char *)Destination;
    const char *src_c = (const char *)Source;
    for (size_t i = 0; i < Size; i++)
    {
        dest_c[i] = src_c[i];
    }
    return Destination;
    /**long d0, d1, d2; 

    asm volatile(
            "rep ; movsq\n\t movq %4,%%rcx\n\t""rep ; movsb\n\t": "=&c" (d0),
            "=&D" (d1),
            "=&S" (d2): "0" (Size >> 3), 
            "g" (Size & 7), 
            "1" (Destination),
            "2" (Source): "memory"
    );  **/
}

#ifndef __aarch64__
struct limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0,
    .response = NULL};

struct limine_kernel_file_request kernel_file_request = {
    .id = LIMINE_KERNEL_FILE_REQUEST,
    .revision = 0,
    .response = NULL};

Page *MemoryAllocator::free_pages = NULL;
page_directory_t *MemoryAllocator::current_directory;
page_directory_t *MemoryAllocator::kernel_directory;

uint64_t MemoryAllocator::kernel_physical_base;
uint64_t MemoryAllocator::kernel_virtual_base;
struct limine_file *MemoryAllocator::kernel_file;
uint64_t MemoryAllocator::kernel_size;

void MemoryAllocator::Init(struct limine_memmap_entry **entries, uint64_t entry_count)
{
    Logger::Log(LogType_Info, "Stating memory initialization...\n");

    Logger::Log(LogType_Debug, "Searching for all usable memory map entries... (This can take a while)");
    uint64_t total_memory = 0;

    for (uint64_t i = 0; i < entry_count; i++)
    {
        struct limine_memmap_entry *entry = entries[i];
        if (entry->type != LIMINE_MEMMAP_USABLE)
        {
            continue;
        }

        total_memory += entry->length;

        uint64_t num_pages = entry->length / PAGE_SIZE;

        for (uint64_t j = 0; j < num_pages; j++)
        {
            uint64_t page_address = entry->base + (j * PAGE_SIZE);
            Page *page = (Page *)page_address;
            page->next = free_pages;
            free_pages = page;
        }
    }
    Logger::PrintOK();

    Logger::Log(LogType_Info, "Memory has been sucessfully initialized!\n");
}

void MemoryAllocator::DetectTotalMemory(struct limine_memmap_entry **entries, uint64_t entry_count)
{
    uint64_t total_memory = 0;

    for (uint64_t i = 0; i < entry_count; i++)
    {
        struct limine_memmap_entry *entry = entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            total_memory += entry->length;
        }
    }

    uint64_t total_memory_in_mb = total_memory / (1024 * 1024);

    Logger::LogFormatted(LogType_Info, "Total usable memory: %lu MB\n", total_memory_in_mb);
}

void *MemoryAllocator::Allocate(size_t size)
{
    Logger::LogFormatted(LogType_Debug, "Attempting to allocate memory of size %lu\n", size);
    size_t num_pages = (size / PAGE_SIZE) + 1;
    Header *header = (Header *)AllocPage();
    if (!header)
    {
        Logger::LogFormatted(LogType_Error, "Failed to allocate header page for malloc.\n");
        return NULL;
    }
    header->num_pages = num_pages;

    for (size_t i = 1; i < num_pages; i++)
    {
        if (!AllocPage())
        {
            Logger::LogFormatted(LogType_Error, "Failed to allocate additional page %llu for malloc.\n", i);
            for (size_t j = i - 1; j > 0; j--)
            {
                FreePage((Page *)((uintptr_t)(header + 1) + j * PAGE_SIZE));
            }
            FreePage((Page *)header);
            return NULL;
        }
    }

    Logger::LogFormatted(LogType_Debug, "Allocated memory block of size %llu starting at 0x%x\n", size, header + 1);
    return header + 1;
}

void MemoryAllocator::Free(void *ptr)
{
    Logger::LogFormatted(LogType_Debug, "Attempting to free memory block at 0x%x\n", ptr);
    Header *header = (Header *)ptr - 1;
    for (size_t i = 0; i < header->num_pages; i++)
    {
        FreePage((Page *)((uintptr_t)(header + 1) + i * PAGE_SIZE));
    }
    Logger::LogFormatted(LogType_Debug, "Freed memory block at 0x%x\n", ptr);
}

Page *MemoryAllocator::AllocPage()
{
    if (!free_pages)
    {
        Logger::LogFormatted(LogType_Error, "No available frames.\n");
        while (1)
        {
            asm("hlt");
        }
    }

    Page *page = free_pages;
    free_pages = free_pages->next;
    page->next = NULL;
    Logger::LogFormatted(LogType_Debug, "Allocated page at address 0x%x\n", page);
    return page;
}

void MemoryAllocator::FreePage(Page *page)
{
    Logger::LogFormatted(LogType_Debug, "Freeing page at address 0x%x\n", page);
    page->next = free_pages;
    free_pages = page;
    Logger::LogFormatted(LogType_Debug, "Freed page at address 0x%x\n", page);
}

void MemoryAllocator::MapPage(void *physaddr, void *virtualaddr, uint32_t flags)
{
    Logger::LogFormatted(LogType_Debug, "Mapping physical address 0x%x to virtual address 0x%x with flags 0x%x\n", (uint64_t)physaddr, (uint64_t)virtualaddr, flags);

    uint32_t dir_idx = PAGE_DIRECTORY_INDEX((uint32_t)virtualaddr);
    uint32_t tbl_idx = PAGE_TABLE_INDEX((uint32_t)virtualaddr);
    Logger::LogFormatted(LogType_Debug, "Directory index: %d, Table index: %d\n", dir_idx, tbl_idx);

    if (current_directory->tables[dir_idx] == NULL)
    {
        current_directory->tables[dir_idx] = (page_table_t *)Allocate(sizeof(page_table_t));
        if (!current_directory->tables[dir_idx])
        {
            Logger::LogFormatted(LogType_Error, "Failed to allocate space for the page table!\n");
            return;
        }
        BasicMemoryManagement::MemorySet32(current_directory->tables[dir_idx], 0, PAGE_SIZE);
    }

    current_directory->tables[dir_idx]->pages[tbl_idx].frame = (uint32_t)physaddr >> 12;
    current_directory->tables[dir_idx]->pages[tbl_idx].present = 1;
    current_directory->tables[dir_idx]->pages[tbl_idx].rw = (flags & 0x2) ? 1 : 0;
    current_directory->tables[dir_idx]->pages[tbl_idx].user = (flags & 0x4) ? 1 : 0;

    Logger::LogFormatted(LogType_Debug, "Mapped physical address 0x%x to virtual address 0x%x successfully\n", (uint64_t)physaddr, (uint64_t)virtualaddr);
}

void MemoryAllocator::UnmapPage(void *virtualaddr)
{
    Logger::LogFormatted(LogType_Debug, "Unmapping virtual address 0x%x\n", (uint64_t)virtualaddr);
    uint32_t dir_idx = PAGE_DIRECTORY_INDEX((uint32_t)virtualaddr);
    uint32_t tbl_idx = PAGE_TABLE_INDEX((uint32_t)virtualaddr);

    if (current_directory->tables[dir_idx] == NULL)
    {
        Logger::LogFormatted(LogType_Warning, "Page table not present for virtual address 0x%x\n", (uint64_t)virtualaddr);
        return;
    }

    current_directory->tables[dir_idx]->pages[tbl_idx].present = 0;
    Logger::LogFormatted(LogType_Debug, "Unmapped virtual address 0x%x successfully\n", (uint64_t)virtualaddr);
}

void MemoryAllocator::SwitchPageDirectory(page_directory_t *new_directory)
{
    Logger::LogFormatted(LogType_Debug, "Switching to page directory at 0x%x\n", (uint64_t)new_directory);
    current_directory = new_directory;
    asm volatile("mov %%cr3, %0"
                 : "=r"(new_directory));
    Logger::LogFormatted(LogType_Debug, "Switched to page directory successfully\n");
}

page_directory_t *MemoryAllocator::CloneDirectory(page_directory_t *src)
{
    Logger::LogFormatted(LogType_Debug, "Cloning page directory at 0x%x\n", (uint64_t)src);
    page_directory_t *dir = (page_directory_t *)Allocate(sizeof(page_directory_t));
    if (!dir)
    {
        Logger::LogFormatted(LogType_Error, "Failed to allocate space for the cloned directory!\n");
        return NULL;
    }
    BasicMemoryManagement::MemorySet32(dir, 0, sizeof(page_directory_t));

    for (int i = 0; i < 1024; i++)
    {
        if (src->tables[i])
        {
            dir->tables[i] = (page_table_t *)Allocate(sizeof(page_table_t));
            if (!dir->tables[i])
            {
                Logger::LogFormatted(LogType_Error, "Failed to allocate space for the cloned page table!\n");
                return NULL;
            }
            BasicMemoryManagement::MemoryCopy(dir->tables[i], src->tables[i], sizeof(page_table_t));
            Logger::LogFormatted(LogType_Debug, "Cloned page table at index %d\n", i);
        }
    }
    Logger::LogFormatted(LogType_Debug, "Cloned page directory successfully to 0x%x\n", (uint64_t)dir);
    return dir;
}

void MemoryAllocator::InitVMM(struct limine_memmap_entry **entries, uint64_t entry_count)
{
    kernel_file = kernel_file_request.response->kernel_file;
    kernel_size = kernel_file->size;

    kernel_physical_base = kernel_address_request.response->physical_base;
    kernel_virtual_base = kernel_address_request.response->virtual_base;

    uint64_t kernel_start = kernel_virtual_base;
    uint64_t kernel_end = kernel_virtual_base + kernel_size;

    kernel_directory = (page_directory_t *)Allocate(sizeof(page_directory_t));
    if (!kernel_directory)
    {
        Logger::LogFormatted(LogType_Error, "Failed to allocate space for the kernel page directory!\n");
        return;
    }
    BasicMemoryManagement::MemorySet32(kernel_directory, 0, sizeof(page_directory_t));

    for (uint64_t i = kernel_start; i < kernel_end; i += PAGE_SIZE)
    {
        MapPage((void *)(i - kernel_start + kernel_physical_base), (void *)i, PAGE_PRESENT | PAGE_RW);
    }

    SwitchPageDirectory(kernel_directory);

    Logger::LogFormatted(LogType_Info, "VMM initialized and kernel pages mapped using PML4 at 0x%llx\n", kernel_directory);
}

}
}
#else

}
}
#endif