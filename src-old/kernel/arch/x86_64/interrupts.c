// SKB_X86_64_ONLY

#include "idt.h"
#include <device/serial/serial.h>
#include <logger/logger.h>

__attribute__((interrupt)) void zero_division_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: division by 0", frame);
}

__attribute__((interrupt)) void debug_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: dbg", frame);
}

__attribute__((interrupt)) void gpf_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: general protection", frame);
}

__attribute__((interrupt)) void pf_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: page", frame);
}

__attribute__((interrupt)) void non_maskable_interrupt_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: non-maskable int", frame);
}

__attribute__((interrupt)) void breakpoint_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: breakpoint", frame);
}

__attribute__((interrupt)) void overflow_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: overflow", frame);
}

__attribute__((interrupt)) void bound_range_exceeded_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: bound range exceeded", frame);
}

__attribute__((interrupt)) void invalid_opcode_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: inv. opcode", frame);
}

__attribute__((interrupt)) void device_not_available_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: dev. not available", frame);
}

__attribute__((interrupt)) void double_fault_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: x2", frame);
}

__attribute__((interrupt)) void invalid_tss_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: tss", frame);
}

__attribute__((interrupt)) void segment_not_present_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: seg. not present", frame);
}

__attribute__((interrupt)) void stack_fault_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: stack", frame);
}

__attribute__((interrupt)) void floating_point_error_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: fpt err", frame);
}

__attribute__((interrupt)) void alignment_check_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: align chk", frame);
}

__attribute__((interrupt)) void machine_check_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: machine chk", frame);
}

__attribute__((interrupt)) void simd_floating_point_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: simd fpt", frame);
}

__attribute__((interrupt)) void virtualization_exception_handler(interrupt_frame *frame)
{
    panic("kernel-mode trap: virtualization", frame);
}

__attribute__((interrupt)) void kb_handler(interrupt_frame *frame)
{
        uint8_t scancode = inb(0x60);
    
        serial_puts("IRQ1 raised");

        pic_end_master();
}

__attribute__((interrupt)) void ms_handler(interrupt_frame *frame)
{
        uint8_t scancode = inb(0x60);
    
        serial_puts("IRQ12 raised");

        pic_end_master();
}

__attribute__((interrupt)) void syscall_handler(interrupt_frame *frame)
{
    log(LT_INFO, "idt:ints", "Interrupt 0x80 (aka. system call) triggered. RAX: 0x%d\n", frame->rax);
}