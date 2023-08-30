#include "memory.h"
#include <stddef.h>
#include <logging/logger.h>
#include <limine/limine.h>

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

uintptr_t PhysicalMemoryManager::Stack[STACK_SIZE];
uint32_t PhysicalMemoryManager::Stack_top;
MemoryBlock *PhysicalMemoryManager::Free_blocks;

uintptr_t PhysicalMemoryManager::kernel_base = 0;
uintptr_t PhysicalMemoryManager::kernel_end = 0;
uintptr_t PhysicalMemoryManager::kernel_size = 0;

static volatile struct limine_memmap_request memmap {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

void PhysicalMemoryManager::Init()
{
    Stack_top = 0;

    for (uint64_t entry; entry < memmap.response->entry_count; entry++)
    {
        limine_memmap_entry *mentry = memmap.response->entries[entry];
        
        if (mentry->type == LIMINE_MEMMAP_USABLE && mentry->length >= PMM_BLOCK_SIZE)
        {
            uintptr_t addr = mentry->base;
            uint64_t length = mentry->length;

            while (length >= PMM_BLOCK_SIZE)
            {
                FreeBlock((void *)addr);
                addr += PMM_BLOCK_SIZE;
                length -= PMM_BLOCK_SIZE;
            }
        }
    }
}

void PhysicalMemoryManager::FreeBlock(void *block)
{
    if (Stack_top == STACK_SIZE)
        return;

    Stack[Stack_top] = (uintptr_t)block;
    Stack_top++;
}

void *PhysicalMemoryManager::AllocBlock()
{
    if (Stack_top == 0)
        return NULL;

    Stack_top--;
    return (void *)Stack[Stack_top];
}

void *PhysicalMemoryManager::MAlloc(size_t size)
{

    if (size == 0)
    {
        return NULL;
    }

    size += sizeof(MemoryBlock);

    MemoryBlock *block = Free_blocks;
    MemoryBlock *prev_block = NULL;

    while (block)
    {
        if (block->is_free && block->size >= size)
        {
            block->is_free = false;

            if (block->size > size + sizeof(MemoryBlock))
            {
                MemoryBlock *new_block = (MemoryBlock *)((uint8_t *)block + size);
                new_block->size = block->size - size;
                new_block->next = block->next;
                new_block->is_free = true;

                block->size = size;
                block->next = new_block;
            }

            return (uint8_t *)block + sizeof(MemoryBlock);
        }

        prev_block = block;
        block = block->next;
    }

    block = (MemoryBlock *)AllocBlock();
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
        Free_blocks = block;
    }

    return (uint8_t *)block + sizeof(MemoryBlock);
}

void PhysicalMemoryManager::Free(void *ptr)
{
    if (!ptr)
    {
        return;
    }

    MemoryBlock *block = (MemoryBlock *)((uint8_t *)ptr - sizeof(MemoryBlock));
    block->is_free = true;

    while (block->next && block->next->is_free)
    {
        block->size += block->next->size;
        block->next = block->next->next;
    }
}

}
}