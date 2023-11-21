/// @brief The main standard library file.
#ifndef __STDLIB_H
#define __STDLIB_H

#include <stdint.h>
#include <stddef.h>

/// @brief Allocate a memory block of the desired size, then return it.
/// @param size The size of the memory block
/// @return The base address of the memory block
void *malloc(size_t size);

/// @brief Free an existing pointer, then allocate a new memory block of the desired size, then return it.
/// @param ptr A pointer to a memory block
/// @param size The size of the new memory block
/// @return The base address of the new memory block
void *realloc(void *ptr, size_t size);

/// @brief Free an existing pointer.
/// @param ptr A pointer to a memory block
void free(void *ptr);

#endif