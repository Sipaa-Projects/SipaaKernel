#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

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

typedef struct pmm_memory_block
{
    size_t size;
    struct pmm_memory_block *next;
    bool is_free;
} pmm_memory_block;

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

void pmm_init();
void *malloc(size_t size);
void free(void *ptr);

#endif