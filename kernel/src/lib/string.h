/*
 * string.h - String management functions. (header)
 *
 * This file is part of the SipaaKernel's standard C library.
 */
#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>

int strcmp(const char *str1, const char *str2);
const char* strstr(const char* haystack, const char* needle);
char *strcpy(char *dest, const char *src);
size_t strlen(const char *str);
char *strdup(const char *s);

#endif