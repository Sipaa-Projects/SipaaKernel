#include <sipaa/syscall.h>
#include <sipaa/logger.h>
#include <sipaa/x86_64/idt.h>
#include <stddef.h>

extern void *Syscall_Print(Syscall_ArgumentsT args);

Syscall_Function syscall_table[SYSCALL_TABLE_SIZE] = {
    Syscall_Print // syscall 0
};

void Syscall_Handler(RegistersT *regs)
{
    Log(LT_INFO, "Syscall", "Handler called\n");
    if (syscall_table[regs->rax] != NULL)
    {
        Syscall_ArgumentsT args;

        args.Argument1 = (void*)regs->rdi;
        args.Argument2 = (void*)regs->rsi;
        args.Argument3 = (void*)regs->rdx;
        args.Argument4 = (void*)regs->r10;
        args.Argument5 = (void*)regs->r8;
        args.Argument6 = (void*)regs->r9;

        regs->rax = syscall_table[regs->rax](args);

        return;
    }

    Log(LT_WARNING, "Syscall", "Invalid syscall: %d\n", regs->rax);
    regs->rax = (void *)-999999999;
}

void Syscall_Initialize()
{
    Idt_SetIsrHandler(128, Syscall_Handler);
}