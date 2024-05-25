#include <sipaa/x86_64/idt.h>
#include <sipaa/logger.h>

void SK_Reboot()
{
    Log(LT_INFO, "PowerMgr", "See you later :)\n");

    // Remove the GPF handler
    Idt_PrepareReboot();

    // Jump to some random bullshit, which causes the hardware to reboot since the GPF handler is inexistant.
    void (*reboot)(void) = (void (*)(void))0xFFFF;
    reboot();
}