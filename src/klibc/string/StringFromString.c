#include <sipaa/libc/string.h>
#include <sipaa/memory.h>
#include <stddef.h>

char *StringFromString(char *s1, char *s2) {
    size_t n = StringLength(s2);
    while(*s1)
        if(!CompareMemory(s1++,s2,n))
            return (char *) (s1-1);
    return 0;
}