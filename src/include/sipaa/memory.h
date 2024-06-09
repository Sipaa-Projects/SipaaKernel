#pragma once

#include <stddef.h>

void *CopyMemory(void *dest, const void *src, size_t n);
void *SetMemory(void *s, int c, size_t n);
void *MoveMemory(void *dest, const void *src, size_t n);
int CompareMemory(const void *s1, const void *s2, size_t n);