#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <sipaa/bootsrv.h>
#include <sipaa/drv/bga.h>
#include <sipaa/pmm.h>
#include <sipaa/kdebug.h>
#include <sipaa/logger.h>
#include <sipaa/fs/vfs.h>
#include <sipaa/fs/hellofs.h>
#include <sipaa/x86_64/gdt.h>
#include <sipaa/x86_64/idt.h>
#include <sipaa/x86_64/vmm.h>
#include <sipaa/bootsrv.h>
#include <sipaa/pci.h>
#include <sipaa/drv/conio.h>
#include <sipaa/framebuffer.h>
#include <sipaa/uptime.h>
#include <sipaa/exec/elf.h>
#include <sipaa/klang.h>

void usr_main() {}

uint64_t kernel_stack[8192];

extern void SK_Reboot();

void SKEntry()
{
    Dbg_Initialize(Com1);
    Fbuf_Initialize();
    ConIO_Initialize();
    ConIO_Print(KLANG_WELCOMETOSK);

    Log(LT_INFO, "Kernel", "Starting initialization\n");
    
    UptimeCounter_Initialize();
    BootSrv_EnumerateProtocolInfos();
    Gdt_Initialize(kernel_stack);
    Idt_Initialize();

    Pmm_Initialize();
    Vmm_Initialize();
    
    Fbuf_InitializeGPU();

    Log(LT_INFO, "Kernel", "Uptime counter is %d.%d\n", UptimeCounter_GetSeconds(), UptimeCounter_GetMilliseconds());
    Log(LT_SUCCESS, "Kernel", "SipaaKernel has been fully initialized. Halting system.\n");

    asm("sti");

    struct limine_file *elf = BootSrv_GetModule(1);
    void (*entry)(void) = (void (*)(void))Elf64_Load((char *)elf->address);
    //entry();

    SK_Reboot();

    for(;;)
    {
        asm("hlt");
    }
}