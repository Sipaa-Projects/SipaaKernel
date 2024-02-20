#include <sipaa/pmm.h>
#include <sipaa/logger.h>
#include <sipaa/entry.h>

/// @brief An array defining a stack for the PMM.
uintptr_t pmm_stack[STACK_SIZE];

/// @brief The top of 'pmm_stack'
uint32_t pmm_stack_top;

/// @brief Free blocks
pmm_memory_block *pmm_free_blocks;

/// @brief The kernel base address
uintptr_t pmm_kernel_base = 0;

/// @brief The kernel end address
uintptr_t pmm_kernel_end = 0;

/// @brief The kernel size
uintptr_t pmm_kernel_size = 0;

/// @brief Free a memory block (internal)
/// @param block The memory block to free
void pmm_free_block(void *block)
{
    if (pmm_stack_top == STACK_SIZE)
        return;

    pmm_stack[pmm_stack_top] = (uintptr_t)block;
    pmm_stack_top++;
}

/// @brief Allocate a new block from free memory (internal)
/// @return A memory block
void *pmm_alloc_block()
{
    if (pmm_stack_top == 0)
        return NULL;

    pmm_stack_top--;
    return (void *)pmm_stack[pmm_stack_top];
}

/// @brief Allocate a memory space of the wanted size
/// @param size The size of the memory block
/// @return The new memory block. NULL if something did go wrong
void *pmm_alloc(size_t size)
{
    if (size == 0)
        return NULL;

    size += sizeof(pmm_memory_block);

    pmm_memory_block *block = pmm_free_blocks;
    pmm_memory_block *prev_block = NULL;

    while (block)
    {
        if (block->is_free && block->size >= size)
        {
            block->is_free = false;

            if (block->size > size + sizeof(pmm_memory_block))
            {
                pmm_memory_block *new_block = (pmm_memory_block *)((uint8_t *)block + size);
                new_block->size = block->size - size;
                new_block->next = block->next;
                new_block->is_free = true;

                block->size = size;
                block->next = new_block;
            }

            return (uint8_t *)block + sizeof(pmm_memory_block);
        }

        prev_block = block;
        block = block->next;
    }

    block = (pmm_memory_block *)pmm_alloc_block();
    if (!block)
    {
        return NULL;
    }

    block->size = size;
    block->is_free = false;
    block->next = NULL;

    if (prev_block)
    {
        prev_block->next = block;
    }
    else
    {
        pmm_free_blocks = block;
    }

    return (uint8_t *)block + sizeof(pmm_memory_block);
}

/// @brief Free a memory space
/// @param ptr The pointer to the memory space
void pmm_free(void *ptr)
{
    if (!ptr)
    {
        return;
    }

    pmm_memory_block *block = (pmm_memory_block *)((uint8_t *)ptr - sizeof(pmm_memory_block));
    block->is_free = true;

    while (block->next && block->next->is_free)
    {
        block->size += block->next->size;
        block->next = block->next->next;
    }
}

/// @brief Reallocate a memory block
/// @param ptr 
/// @param size 
/// @return 
void* pmm_realloc(void* ptr, size_t size) {
    if (size == 0) {
        pmm_free(ptr);
        return NULL;
    }

    void* new_ptr = pmm_alloc(size);
    if (new_ptr == NULL) {
        return NULL;  // Allocation failed
    }

    // Copy data from the old block to the new block
    size_t old_size = 0;
    if (ptr) {
        // Find the size of the old block
        void* temp_ptr = ptr;
        while (*(char*)temp_ptr != '\0') {
            temp_ptr++;
            old_size++;
        }
        old_size++;  // Include the null terminator
        // Copy the data
        for (size_t i = 0; i < old_size; i++) {
            *((char*)new_ptr + i) = *((char*)ptr + i);
        }
    }

    pmm_free(ptr);  // Free the old block

    return new_ptr;
}

/// @brief Initialize the PMM.
void pmm_init(sk_general_boot_info skgbi) {
    log(LT_INFO, "PhysMM", "Initializing...\n");

    pmm_stack_top = 0;
    
    for (uint64_t entry = 0; entry < skgbi.memory_map.length; entry++)
    {
        sk_memory_mapentry mentry = skgbi.memory_map.entries[entry];
        
        if (mentry.type == MEMMAP_USABLE || mentry.type == MEMMAP_BOOTLOADER_RECLAIMABLE && mentry.length >= PMM_BLOCK_SIZE) // poor limine
        {
            uintptr_t addr = mentry.base;
            uint64_t length = mentry.length;

            while (length >= PMM_BLOCK_SIZE)
            {
                pmm_free_block((void *)addr);
                addr += PMM_BLOCK_SIZE;
                length -= PMM_BLOCK_SIZE;
            }
        }
    }

    log(LT_INFO, "PhysMM", "Initialized\n"); // thanks googel
}