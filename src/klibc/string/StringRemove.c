#include <sipaa/libc/string.h>
#include <sipaa/memory.h>

void StringRemove(char *str, char *sub) {
    char *p = StringFromString(str, sub);
    if (p) {
        MoveMemory(p, p + StringLength(sub), StringLength(p + StringLength(sub)) + 1);
    }
}