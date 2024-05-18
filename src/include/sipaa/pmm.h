#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096

#define DIV_ROUND_UP(x, y) (x + (y - 1)) / y
#define ALIGN_UP(x, y) DIV_ROUND_UP(x, y) * y
#define ALIGN_DOWN(x, y) (x / y) * y

struct PmmNode
{
    uint64_t Base;
    uint64_t Flags;
    struct PmmNode *Next;
};
typedef struct PmmNode PmmNodeT;

/// @brief Initialize the PMM.
void Pmm_Initialize();

/// @brief Allocates a page of memory.
/// @return A pointer to the new page.
void *Pmm_AllocatePage();

/// @brief Free an allocated page of memory
/// @param page A pointer to the page
void Pmm_FreePage(void *page);

/// @brief Allocate a memory block of the requested size.
/// @param size The size of the new memory block 
/// @return A pointer to the memory block (COULD BE NULL!)
void *Pmm_Allocate(size_t size);

/// @brief Free an allocated memory block
/// @param ptr The pointer to the memory block.
void Pmm_Free(void *ptr);

/// @brief Reallocate a memory block
/// @param ptr A pointer to the old memory block
/// @param size The new size of the memory block
/// @return The new memory block
void *Pmm_Reallocate(void* ptr, size_t size);