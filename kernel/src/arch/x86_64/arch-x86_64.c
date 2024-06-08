#ifdef __x86_64__

#include <sipaakernel/arch.h>

void Arch_HCF()
{
    for (;;)
        asm("hlt"); // Halt
}

#endif