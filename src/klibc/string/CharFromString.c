#include <sipaa/libc/string.h>

char *CharFromString(const char *s, int c) {
    while (*s != (char) c) {
        if (!*s++) {
            return NULL;
        }
    }
    return (char *)s;
}