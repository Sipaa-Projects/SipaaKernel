#include <arch/x86_64/gdt.h>
#include <arch/x86_64/idt.h>
#include <device/conio/conio.h>
#include <device/pci/pci.h>
#include <device/ps2/ps2.h>
#include <device/serial/serial.h>
#include <fs/mountmgr.h>
#include <impl/logger-impl.h>
#include <logger/logger.h>
#include <memory/pmm.h>
#include <slibc/stdio.h>
#include <slibc/assert.h>
#include <stdint.h>

#ifndef __i686__
uint64_t kernel_stack[8192];
#endif

void sk_boot()
{
    pmm_init();

    #if defined(__x86_64__) | defined(__i686__ ) | defined(__i386__)
    init_serial();
    #endif
    logger_sk_impl_init();
    conio_initialize();

    #ifdef __x86_64__
    init_gdt(kernel_stack);
    init_idt();
    #endif

    // Mount kernel file systems
    mount("devfs", "/dev", FST_DEVFS);
}

extern int emergshell_main();

void _start()
{
    sk_boot();

    log(LT_INFO, "post-boot", "No 'init' executable file found. Jumping to emergency shell.\n");

    //emergshell_main();

    panic("Reached end of _start.", NULL);
}