#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <sipaa/bootsrv.h>
#include <sipaa/drv/bga.h>
#include <sipaa/pmm.h>
#include <sipaa/kdebug.h>
#include <sipaa/logger.h>
//#include <sipaa/fs/ramdisk.h>
#include <sipaa/x86_64/gdt.h>
#include <sipaa/x86_64/idt.h>
#include <sipaa/x86_64/vmm.h>
#include <sipaa/pci.h>
#include <sipaa/framebuffer.h>

void usr_main() {}

uint64_t kernel_stack[8192];

void SKEntry()
{
    Dbg_Initialize(Com1);

    Log(LT_INFO, "Kernel", "We are here\n");
    
    BootSrv_EnumerateProtocolInfos();
    Gdt_Initialize(kernel_stack);
    Idt_Initialize();

    Pmm_Initialize();
    vmm_init();

    // At this point, SK should crash since the framebuffer doesn't use virtual addresses right now.
    /**Fbuf_Initialize();

    FramebufferT *fb = Fbuf_Get();
    uint32_t *fbufaddr = fb->Address;
    
    for (int i = 0; i < fb->Size; i++)
        fbufaddr[i] = 0xFFFFFF;**/

    for(;;) ;;
}