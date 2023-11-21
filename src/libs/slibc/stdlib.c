#include "stdlib.h"
#include "sysi.h"

/// @brief A pointer to the current system interface, defined in libc-init.c
extern system_i *current_sysi;

void *malloc(size_t size)
{
    return current_sysi->malloc(size);
}

void free(void *ptr)
{
    return current_sysi->free(ptr);
}