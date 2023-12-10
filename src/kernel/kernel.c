#include <arch/x86_64/gdt.h>
#include <arch/x86_64/idt.h>
#include <limine/limine.h>
#include <stdint.h>
#include "logger-impl.h"

uint64_t kernel_stack[8192];

void _start(void)
{
    //init_video(framebuffer_request.response->framebuffers[0]);
    logger_sk_impl_init();
    flanterm_write(logger_ftctx, "Welcome to SipaaKernel!\n\n", 24);

    #ifdef __x86_64__
    init_gdt(kernel_stack);
    init_idt();
    #endif
    
    while (1) { }
}