#include <sk-hal/hal.h>
#include <sk-logger/logger.h>
#include "gdt.h"
#include "idt.h"
#include "rtc.h"

uint64_t kernel_stack[8192];

int hal_init(void) {
    int r = 0;
    gdt_init((uint64_t)kernel_stack);
    idt_init();
    r = rtc_init();
    return r;
}

void hal_cpu_sti(void) {
    log(LT_INFO, "enabling interrupts\n");
    asm("sti");
}

void hal_cpu_cli(void) {
    log(LT_INFO, "disabling interrupts\n");
    asm("cli");
}

void hal_cpu_halt(void) {
    log(LT_INFO, "halting CPU\n");
    asm("hlt");
}

void hal_cpu_relax(void) {
    log(LT_INFO, "relaxing CPU\n");
    asm("pause");
}

void hal_cpu_stop(void) {
    while (1) {
        //hal_cpu_cli();
        hal_cpu_halt();
        
    }
}

void hal_register_idt_handler(int num, void* handler, uint8_t ist)
{
    idt_set_entry(num, handler, ist);
}