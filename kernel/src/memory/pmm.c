#include <memory/memory.h>
#include <boot/limine.h>
#include <lib/log.h>

uptr pmm_stack[STACK_SIZE];
ui32 pmm_stack_top;
pmm_memory_block *pmm_free_blocks;

uptr pmm_kernel_base = 0;
uptr pmm_kernel_end = 0;
uptr pmm_kernel_size = 0;

static volatile struct limine_memmap_request memmap = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

void pmm_init()
{
    pmm_stack_top = 0;

    for (ui64 entry = 0; entry < memmap.response->entry_count; entry++)
    {
        struct limine_memmap_entry *mentry = memmap.response->entries[entry];
        
        if (mentry->type == LIMINE_MEMMAP_USABLE && mentry->length >= PMM_BLOCK_SIZE)
        {
            //log(LOGTYPE_INFO, "pmm: found usable memory entry");
            uptr addr = mentry->base;
            ui64 length = mentry->length;

            while (length >= PMM_BLOCK_SIZE)
            {
                pmm_free_block((void *)addr);
                addr += PMM_BLOCK_SIZE;
                length -= PMM_BLOCK_SIZE;
            }
        }
    }

    //log(LOGTYPE_SUCCESS, "pmm: loaded successfully");
}

void pmm_free_block(void *block)
{
    if (pmm_stack_top == STACK_SIZE)
        return;

    pmm_stack[pmm_stack_top] = (uptr)block;
    pmm_stack_top++;
}

void *pmm_alloc_block()
{
    if (pmm_stack_top == 0)
        return NULL;

    pmm_stack_top--;
    return (void *)pmm_stack[pmm_stack_top];
}

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
                pmm_memory_block *new_block = (pmm_memory_block *)((ui8 *)block + size);
                new_block->size = block->size - size;
                new_block->next = block->next;
                new_block->is_free = true;

                block->size = size;
                block->next = new_block;
            }

            return (ui8 *)block + sizeof(pmm_memory_block);
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

    return (ui8 *)block + sizeof(pmm_memory_block);
}

void pmm_free(void *ptr)
{
    if (!ptr)
    {
        return;
    }

    pmm_memory_block *block = (pmm_memory_block *)((ui8 *)ptr - sizeof(pmm_memory_block));
    block->is_free = true;

    while (block->next && block->next->is_free)
    {
        block->size += block->next->size;
        block->next = block->next->next;
    }
}