#include <sipaa/syscall.h>
#include <sipaa/i686/isr.h>
#include <sipaa/logger.h>
#include <stdint.h>

SYSCALL_F syscalls[SYSCALL_TABLE_LENGTH];

void syscall_handler(REGISTERS *r)
{
    SYSCALL_F callfunc = syscalls[r->eax];
    if (callfunc == (SYSCALL_F)0)
    {
        log(LT_WARNING, "SysCall", "No system call has been registered with ID: %d (EAX)\n", r->eax)
        return;
    }
    callfunc(r);
}

void register_syscall(int id, SYSCALL_F handler)
{
    if (syscalls[id] != (SYSCALL_F)0)
    {
        return;
    }
    syscalls[id] = handler;
}

int syscall_init()
{
    for (size_t i = 0; i < SYSCALL_TABLE_LENGTH; i++)
    {
        syscalls[i] = (SYSCALL_F)0;
    }

    isr_register_interrupt_handler(0x80, syscall_handler);
    return 0;
}