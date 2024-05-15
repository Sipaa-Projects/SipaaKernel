#include <sipaa/libc/string.h>

int CompareStrings(const char *str1, const char *str2)
{
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }

    return *(const unsigned char *)str1 - *(const unsigned char *)str2;
}
