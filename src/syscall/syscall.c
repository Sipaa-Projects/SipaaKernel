#include <sipaa/syscall.h>

extern void *Syscall_Print(Syscall_ArgumentsT args);

Syscall_Function syscall_table[SYSCALL_TABLE_SIZE] = {
    Syscall_Print // syscall 0
};
