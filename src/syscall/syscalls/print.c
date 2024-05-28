#include <sipaa/syscall.h>

#include <sipaa/drv/conio.h>

void *Syscall_Print(Syscall_ArgumentsT args)
{
    ConIO_Print((char *)args.Argument1);

    return (void *)0;
}