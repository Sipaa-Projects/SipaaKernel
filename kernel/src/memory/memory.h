#pragma once

#include <stdint.h>
#include <stddef.h>

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

#define PMM_BLOCK_SIZE 4096
#define PMM_MAX_BLOCKS 32768
#define STACK_SIZE PMM_MAX_BLOCKS

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

typedef struct memory_block
{
        size_t size;
        struct memory_block *next;
        bool is_free;
} MemoryBlock;

class PhysicalMemoryManager {

private:
    static uintptr_t Stack[STACK_SIZE];
    static uint32_t Stack_top;
    static MemoryBlock *Free_blocks = NULL;

    static uintptr_t kernel_base;
    static uintptr_t kernel_end;
    static uintptr_t kernel_size;

public:
    static void Init();
    static void FreeBlock(void *block);
    static void *AllocBlock();
    static void Free(void *ptr);
    static void *MAlloc(size_t size);

};

}
}