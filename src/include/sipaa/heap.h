#pragma once

#include <stdint.h>

#define HEAP_MAGIC 0x719A5EA9 // SIPAHEAP

typedef struct Heap_Block
{
    struct Heap_Block *next;
    struct Heap_Block *prev;
    uint8_t state;
    uint32_t magic;
    uint64_t size;
} Heap_BlockT;

typedef struct Heap
{
    Heap_BlockT *block_head;
} HeapT;

extern HeapT *kernel_heap;

void KHeap_Initialize();
void *KHeap_Allocate(uint64_t size);
void KHeap_Free(void *ptr);