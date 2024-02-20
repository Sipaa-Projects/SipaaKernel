#include <sipaa/conio.h>
#include <sipaa/kdebug.h>

#define __i686__

#ifdef __i686__
#include <sipaa/i686/isr.h>
#endif

void panic(char *message)
{
    dbg_system_panic();
    
    conio_chbg(0xFF0000FF);
    conio_chfg(0xFFFFFF);

    conio_clear();

    conio_println("A problem has been detected and SipaaKernel has been stopped to prevent any damages to your PC."); 
    conio_println("If this is the first time you see this stop error screen, restart your computer. if this screen appears again, follow these steps:");
    conio_println("    * Try tweaking BIOS settings.");
    conio_println("    * Try fixing the issue manually in the code"); 
    conio_println("    * Try reporting the issue on GitHub (https://github.com/Sipaa-Projects/SipaaKernel)\n");
    conio_println("Technical informations:");
    conio_printfln("Error Message: %s", message);

    #ifdef __i686__
    //conio_printf("Interrupt which triggered this stop screen: %d\n", regs.int_no);
    //conio_printf("CPU registers: \nds: %x\nedi: %x, esi: %d, ebp: %d, esp: %d, ebx: %d, edx: %d, ecx: %d, eax: %d", regs.ds, regs.edi, regs.esi, regs.ebp, regs.esp, regs.ebx, regs.edx, regs.ecx, regs.eax);
    #endif

    asm("hlt");
    for (;;)
        ;
}