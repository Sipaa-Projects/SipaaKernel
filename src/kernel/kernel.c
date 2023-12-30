#include <arch/x86_64/gdt.h>
#include <arch/x86_64/idt.h>
#include <entry/skgbi.h>
#include <stdint.h>
#include <logger-impl.h>
#include <logger/logger.h>
#include <prestart/sk_prestart_return.h>
#include <device/conio/conio.h>
#include <device/serial/serial.h>
#include <slibc/stdio.h>

#ifndef __i686__
uint64_t kernel_stack[8192];
#endif

int _start(sk_general_boot_info skgbi)
{
    #if defined(__x86_64__) | defined(__i686__)
    init_serial();
    #endif
    conio_initialize(skgbi);
    conio_write("Welcome to SipaaKernel!\n\n", sizeof(char), 24);
    logger_sk_impl_init();

    #ifdef __x86_64__
    #ifdef SKC_ENABLEPCIC
    log(LT_INFO, "kernel", "PCI compilation was enabled.\n")
    #else
    log(LT_INFO, "kernel", "PCI compilation wasn't enabled.\n");
    #endif
    init_gdt(kernel_stack);
    init_idt();
    #endif

    printf("hi");

    return SK_RETURN_LOOP;
}