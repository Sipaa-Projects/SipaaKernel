#ifndef __BMO_H
#define __BMO_H

#include <stdint.h>
#include <stddef.h>

/// @brief Copy a memory block from source to destination
/// @param dest Destination
/// @param src Source
/// @param n Size
/// @return Destination
void *memcpy(void *dest, const void *src, size_t n);

/// @brief Fill a memory block with a 32bit-signed integer value.
/// @param s The memory block to fill
/// @param c The value to fill with.
/// @param n The size of the memory block
/// @return The memory block to fill
void *memset(void *s, int c, size_t n);

/// @brief Move a memory block from source to destination
/// @param dest Destination
/// @param src Source
/// @param n Size
/// @return Destination
void *memmove(void *dest, const void *src, size_t n);

/// @brief Compare two memory blocks
/// @param s1 The first memory block
/// @param s1 The second memory block
/// @param n The size of these 2 memory blocks to compare
/// @return 0 if there's no difference between the 2 memory blocks.
int memcmp(const void *s1, const void *s2, size_t n);

#endif