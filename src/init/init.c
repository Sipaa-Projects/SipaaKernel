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
    //RamDisk_Initialize();

    asm("int $0x03");

    for(;;) ;;
}