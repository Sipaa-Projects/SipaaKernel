#pragma once

#include <stddef.h>
#include <stdbool.h>

/// @brief The default size for a page
#define PAGE_SIZE 4096
/// @brief The entries of the page table
#define PAGE_TABLE_ENTRIES 512
/// @brief Flags for a page
#define PAGE_FLAGS 0xFFF
/// @brief Check if page present
#define PAGE_PRESENT 0x1
/// @brief Read/write page
#define PAGE_RW 0x2
/// @brief "User" page
#define PAGE_USER 0x4
/// @brief 2mb page
#define PAGE_2MB 0x80
/// @brief Page directory index
#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3FF)
/// @brief Page table index
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3FF)
/// @brief Get page address
#define GET_PAGE_ADDRESS(x) ((x) & ~0xFFF)

#define DIV_ROUND_UP(x, y) (x + (y - 1)) / y
#define ALIGN_UP(x, y) DIV_ROUND_UP(x, y) * y
#define ALIGN_DOWN(x, y) (x / y) * y

/// @brief The default size for a page block
#define PMM_BLOCK_SIZE 4096
/// @brief The max blocks
#define PMM_MAX_BLOCKS 32768
/// @brief PMM_MAX_BLOCKS
#define STACK_SIZE PMM_MAX_BLOCKS

/// @brief A memory block.
typedef struct Pmm_MemoryBlock
{
    /// @brief The size
    size_t size;
    /// @brief The next memory block
    struct Pmm_MemoryBlock *next;
    /// @brief Used to check if the memory block is free
    bool is_free;
} Pmm_MemoryBlockT;

#ifdef __cplusplus
extern "c" {
#endif

void Pmm_Initialize();
void *Pmm_Allocate(size_t size);
void *Pmm_AllocatePage();
void *Pmm_Reallocate(void *ptr, size_t size);
void Pmm_Free(void *ptr);
void Pmm_FreePage(void *ptr);

#ifdef __cplusplus
}
#endif