#include <memory/memory.h>

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!

void *memcpy(void *dest, const void *src, size_t n) {
    ui8 *pdest = (ui8 *)dest;
    const ui8 *psrc = (const ui8 *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    ui8 *p = (ui8 *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (ui8)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    ui8 *pdest = (ui8 *)dest;
    const ui8 *psrc = (const ui8 *)src;

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
    const ui8 *p1 = (const ui8 *)s1;
    const ui8 *p2 = (const ui8 *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}