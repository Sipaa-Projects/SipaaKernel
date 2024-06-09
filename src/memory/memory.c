#include <stddef.h>
#include <stdint.h>
#include <sipaa/memory.h>

/* Since the modern compilers reserve the right to add calls to these 4 functions, keep their name intact, and add 4 new functions with SK naming. */

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *pdest = dest;
    const uint8_t *psrc = src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = dest;
    const uint8_t *psrc = src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = s1;
    const uint8_t *p2 = s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i])
            return p1[i] < p2[i] ? -1 : 1;
    }

    return 0;
}

void *CopyMemory(void *dest, const void *src, size_t n) {
    return memcpy(dest, src, n);
}

void *SetMemory(void *s, int c, size_t n) {
    return memset(s, c, n);
}

void *MoveMemory(void *dest, const void *src, size_t n) {
    return memmove(dest, src, n);
}

int CompareMemory(const void *s1, const void *s2, size_t n) {
    return memcmp(s1, s2, n);
}