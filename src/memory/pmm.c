#include <sipaa/pmm.h>
#include <sipaa/bootsrv.h>
#include <sipaa/logger.h>
#include <limine.h>

PmmNodeT *Pmm_Head = NULL;

void Pmm_Initialize()
{
    Log(LT_INFO, "Pmm", "Starting initialization\n");
    PmmNodeT *prev_node = NULL;

    struct limine_memmap_response *mmap = BootSrv_GetMemoryMap();

    for (int i = 0; i < mmap->entry_count; i++)
    {
        struct limine_memmap_entry *entry = mmap->entries[i];
        uint64_t length_mb = entry->length / 1024;
        Log(LT_INFO, "Pmm", "Entry: Type: %d, Base: 0x%x, Size: %dkb\n", entry->type, entry->base, length_mb);
        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            for (uint64_t j = 0; j < entry->length; j += PAGE_SIZE)
            {
                PmmNodeT *cur_node = (PmmNodeT *)(entry->base + j);
                cur_node->Base = entry->base + j;
                cur_node->Flags = 0;
                cur_node->Next = NULL;
                if (prev_node != NULL)
                {
                    prev_node->Next = cur_node;
                }
                else
                {
                    Pmm_Head = cur_node;
                }
                prev_node = cur_node;
            }
        }
    }
}

void *Pmm_AllocatePage()
{
    PmmNodeT *node = Pmm_Head;
    if (node != NULL)
    {
        Pmm_Head = node->Next;
        node->Next = NULL;
        node->Flags = 1;
        return (void *)node->Base;
    }
    
    return NULL;
}

void Pmm_FreePage(void *page)
{
    PmmNodeT *node = (PmmNodeT *)page;
    node->Flags = 0;
    node->Next = Pmm_Head;
    Pmm_Head = node;
}

void *Pmm_Allocate(size_t size)
{
    size_t num_pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;

    PmmNodeT *prev_node = NULL;
    PmmNodeT *cur_node = Pmm_Head;
    while (cur_node != NULL && num_pages > 0)
    {
        if (cur_node->Flags == 0)
        {
            cur_node->Flags = 1;
            num_pages--;

            if (prev_node == NULL)
            {
                Pmm_Head = cur_node->Next;
            }
            else
            {
                prev_node->Next = cur_node->Next;
            }

            prev_node = cur_node;
            cur_node = cur_node->Next;
        }
        else
        {
            prev_node = cur_node;
            cur_node = cur_node->Next;
        }
    }

    if (num_pages == 0)
    {
        Log(LT_DEBUG, "Pmm", "Allocated memory block: Size: %d, Base: %p\n", size, (void *)prev_node->Base);
        return (void *)prev_node->Base;
    }

    return NULL;
}

void Pmm_Free(void *ptr)
{
    uint64_t addr = (uint64_t)ptr & ~(PAGE_SIZE - 1);

    PmmNodeT *node = Pmm_Head;

    while (node != NULL && node->Base < addr)
    {
        node = node->Next;
    }

    if (node != NULL && node->Base == addr)
    {
        Pmm_Head = node->Next;
    }

    node->Flags = 0;

    node->Next = Pmm_Head;
    Pmm_Head = node;

    Log(LT_DEBUG, "Pmm", "Freed memory block at %p", ptr);
}

void* Pmm_Reallocate(void* ptr, size_t size) {
    if (size == 0) {
        Pmm_Free(ptr);
        return NULL;
    }

    void* new_ptr = Pmm_Allocate(size);
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

    Pmm_Free(ptr);  // Free the old block

    return new_ptr;
}