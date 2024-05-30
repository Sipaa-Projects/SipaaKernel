#ifdef __x86_64__

#include <sipaa/x86_64/idt.h>
#include <sipaa/logger.h>

void SK_Reboot()
{
    Log(LT_INFO, "PowerMgr", "See you later :)\n");

    // Make the IDT know that we want to reboot the PC, so he can disable the interrupt handlers.
    Idt_PrepareReboot();

    // Jump to some random bullshit, which causes the hardware to reboot since the GPF handler is inexistant.
    void (*reboot)(void) = (void (*)(void))0xFFFF;
    reboot();
}

#endif