/*
 * stdlib.c - Standard functions.
 *
 * This file is part of the SipaaKernel's standard C library.
 */

#include "stdlib.h"
#include <memory/memory.h>

ui32 rand_seed = 0;

void *malloc(i32 size) {
    return pmm_alloc(size);
}

void *realloc(void *ptr, i32 size) {
    pmm_free(ptr);
    return pmm_alloc(size);
}

void *calloc(ui32 nmemb, ui32 size) {
    ui32 total_size = nmemb * size;

    void *ptr = pmm_alloc(total_size);

    if (ptr != NULL) {
        memset(ptr, 0, total_size);
    }

    return ptr;
}

void free(void *ptr)
{
    pmm_free(ptr);
}

void srand(ui32 seed)
{
    rand_seed = seed;
}

int rand(int max)
{
    rand_seed = (rand_seed * 1103515245 + 12345) & 0x7FFFFFFF; // Linear congruential generator
    return rand_seed % max;
}