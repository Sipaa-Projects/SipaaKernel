#pragma once

#include <cstdint>
#include <cstddef>

#include <limine/limine.h>

#define PAGE_SIZE 4096
#define PAGE_FLAGS 0xFFF
#define PAGE_PRESENT 0x1
#define PAGE_RW 0x2
#define PAGE_USER 0x4
#define PAGE_2MB 0x80
#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3FF)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3FF)
#define GET_PAGE_ADDRESS(x) ((x) & ~0xFFF)

namespace Sk {
namespace Memory {

/// @brief Class used to do basic operations with memory.
class BasicMemoryManagement {

public:
    static int MemoryCompare(const void *s1, const void *s2, size_t n);
    static void MemorySet(void* start, uint8_t value, size_t size);
    static void MemorySet16(void* start, uint16_t value, size_t size);
    static void MemorySet32(void* start, uint32_t value, size_t size);
    static void MemorySet64(void* start, uint64_t value, size_t size);
    static void MemoryCopy(void* Destination, void* Source, size_t Size);

};

typedef struct page
{
    uint32_t present : 1;
    uint32_t rw : 1;
    uint32_t user : 1;
    uint32_t accessed : 1;
    uint32_t dirty : 1;
    uint32_t unused : 7;
    uint32_t frame : 20;
} page_t;

typedef struct page_table
{
    page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
    page_table_t *tables[1024];
    uint32_t tablesPhysical[1024];
    uint32_t physicalAddr;
} page_directory_t;

typedef struct Page
{
    struct Page *next;
} Page;

typedef struct
{
    size_t num_pages;
} Header;

/// @brief Class used for allocating/freeing memory
class MemoryAllocator {
public:
    static Page *free_pages;
    static page_directory_t *current_directory;
    static page_directory_t *kernel_directory;

    static uint64_t kernel_physical_base;
    static uint64_t kernel_virtual_base;
    static struct limine_file *kernel_file;
    static uint64_t kernel_size;

    static void Init(struct limine_memmap_entry **entries, uint64_t entry_count);
    static void DetectTotalMemory(struct limine_memmap_entry **entries, uint64_t entry_count);
    static void *Allocate(size_t size);
    static void Free(void *ptr);
    static Page *AllocPage();
    static void FreePage(Page *page);
    static void MapPage(void *physaddr, void *virtualaddr, uint32_t flags);
    static void UnmapPage(void *virtualaddr);
    static void SwitchPageDirectory(page_directory_t *new_directory);
    static page_directory_t *CloneDirectory(page_directory_t *src);
    static void InitVMM(struct limine_memmap_entry **entries, uint64_t entry_count);
};

} // namespace name    
} // namespace Sk