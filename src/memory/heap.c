#include <sipaa/heap.h>
#include <sipaa/logger.h>
#include <sipaa/memory.h>
#include <sipaa/x86_64/vmm.h>
#include <sipaa/pmm.h>

HeapT *kernel_heap;

void KHeap_Initialize() {
    HeapT *h = (HeapT *)VIRTUAL_TO_PHYSICAL(Pmm_Allocate(1));
    h->block_head = (HeapT *)VIRTUAL_TO_PHYSICAL(Pmm_Allocate(1));
    h->block_head->magic = HEAP_MAGIC;
    h->block_head->next = h->block_head->prev = h->block_head;
    h->block_head->size = 0;
    h->block_head->state = 1;

    kernel_heap = h;

    Log(LT_SUCCESS, "KHeap", "Initialized!\n");
}

void *KHeap_Allocate(uint64_t size) {
    Heap_BlockT *current = kernel_heap->block_head;

    do {
        if (current->state == 0 && current->size >= size) {
            current->state = 1;
            return (void *)((uint64_t)current + sizeof(Heap_BlockT));
        }
        current = current->next;
    } while (current != kernel_heap->block_head);

    Heap_BlockT *new_block = (Heap_BlockT *)VIRTUAL_TO_PHYSICAL(Pmm_Allocate(1));
    new_block->magic = HEAP_MAGIC;
    new_block->size = size;
    new_block->state = 1;

    new_block->next = kernel_heap->block_head;
    new_block->prev = kernel_heap->block_head->prev;
    kernel_heap->block_head->prev->next = new_block;
    kernel_heap->block_head->prev = new_block;

    return (void *)((uint64_t)new_block + sizeof(Heap_BlockT));
}

void KHeap_Free(void *ptr) {
    if (!ptr) return;

    Heap_BlockT *block = (Heap_BlockT *)((uint64_t)ptr - sizeof(Heap_BlockT));

    if (block->magic != HEAP_MAGIC) return;

    block->state = 0;

    if (block->next != kernel_heap->block_head && block->next->state == 0) {
        block->size += sizeof(Heap_BlockT) + block->next->size;
        block->next = block->next->next;
        block->next->prev = block;
    }

    if (block->prev != kernel_heap->block_head && block->prev->state == 0) {
        block->prev->size += sizeof(Heap_BlockT) + block->size;
        block->prev->next = block->next;
        block->next->prev = block->prev;
    }
}