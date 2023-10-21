/*
 * stdlib.h - Standard functions. (header)
 *
 * This file is part of the SipaaKernel's standard C library.
 */

#ifndef __stdlib_h__
#define __stdlib_h__

#include <lib/stdtype.h>

void *malloc(i32 size);
void *realloc(void *ptr, i32 size);
void *calloc(ui32 nmemb, ui32 size);
void free(void *ptr);
void srand(ui32 seed);
int rand(int max);

#endif
