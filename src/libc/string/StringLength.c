#include <sipaa/libc/string.h>

int StringLength(char *s)
{
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}
