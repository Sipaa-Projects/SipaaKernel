#include <arch/x86_64/gdt.h>
#include <arch/x86_64/idt.h>
#include <memory/pmm.h>
#include <entry/skgbi.h>
#include <stdint.h>
#include <logger-impl.h>
#include <logger/logger.h>
#include <prestart/sk_prestart_return.h>

#ifndef __i686__
uint64_t kernel_stack[8192];
#endif

int _start(sk_general_boot_info skgbi)
{
    //init_video(framebuffer_request.response->framebuffers[0]);
    logger_sk_impl_init(skgbi);
    flanterm_write(logger_ftctx, "Welcome to SipaaKernel!\n\n", 24);

    pmm_init(skgbi);

    #ifdef __x86_64__
    #ifdef SKC_ENABLEPCIC
    log(LT_INFO, "kernel", "PCI compilation was enabled.\n")
    #else
    log(LT_INFO, "kernel", "PCI compilation wasn't enabled.\n");
    #endif
    init_gdt(kernel_stack);
    init_idt();
    #endif

    return SK_RETURN_LOOP;
}