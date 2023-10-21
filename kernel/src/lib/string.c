/*
 * string.c - String management functions.
 *
 * This file is part of the SipaaKernel's standard C library.
 */
#include <lib/string.h>

#include <memory/memory.h>
#include <stddef.h>

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }

    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

size_t strlen(const char *str) {
    size_t length = 0;
    while (*str++) {
        ++length;
    }
    return length;
}

char *strcpy(char *dest, const char *src) {
    char *original_dest = dest;
    while ((*dest++ = *src++)) {
        // Empty body; the work is done in the condition
    }
    return original_dest;
}

char *strdup(const char *s) {
    // Allocate memory for the new string (plus the null-terminator)
    char *new_str = (char *)malloc(strlen(s) + 1);
    
    // Check for memory allocation failure
    if (new_str == NULL) {
        return NULL;
    }
    
    // Copy the string into the new memory location
    strcpy(new_str, s);
    
    return new_str;
}

const char* strstr(const char* haystack, const char* needle) {
    if (!*needle) {
        return haystack;
    }

    while (*haystack) {
        const char *h = haystack;
        const char *n = needle;

        while (*h && *n && (*h == *n)) {
            ++h;
            ++n;
        }

        if (!*n) {
            return haystack;
        }

        ++haystack;
    }
    return NULL;
}

