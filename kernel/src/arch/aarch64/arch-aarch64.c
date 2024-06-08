#ifdef __aarch64__

#include <sipaakernel/arch.h>

void Arch_HCF()
{
    for (;;)
        asm("wfi"); // Wait For Interrupt
}

#endif