#include "idt.h"

__attribute__((interrupt)) void zero_division_handler(struct interrupt_frame *frame)
{
    panic("DIVISION BY ZERO", frame);
}

__attribute__((interrupt)) void debug_handler(struct interrupt_frame *frame)
{
    panic("DEBUG", frame);
}

__attribute__((interrupt)) void gpf_handler(struct interrupt_frame *frame)
{
    panic("GENERAL PROTECTION FAULT", frame);
}

__attribute__((interrupt)) void pf_handler(struct interrupt_frame *frame)
{
    panic("PAGE FAULT", frame);
}

__attribute__((interrupt)) void non_maskable_interrupt_handler(struct interrupt_frame *frame)
{
    panic("NON-MASKABLE INTERRUPT", frame);
}

__attribute__((interrupt)) void breakpoint_handler(struct interrupt_frame *frame)
{
    panic("BREAKPOINT", frame);
}

__attribute__((interrupt)) void overflow_handler(struct interrupt_frame *frame)
{
    panic("OVERFLOW", frame);
}

__attribute__((interrupt)) void bound_range_exceeded_handler(struct interrupt_frame *frame)
{
    panic("BOUND RANGE EXCEEDED", frame);
}

__attribute__((interrupt)) void invalid_opcode_handler(struct interrupt_frame *frame)
{
    panic("INVALID OPCODE", frame);
}

__attribute__((interrupt)) void device_not_available_handler(struct interrupt_frame *frame)
{
    panic("DEVICE NOT AVAILABLE", frame);
}

__attribute__((interrupt)) void double_fault_handler(struct interrupt_frame *frame)
{
    panic("DOUBLE FAULT", frame);
}

__attribute__((interrupt)) void invalid_tss_handler(struct interrupt_frame *frame)
{
    panic("INVALID TSS", frame);
}

__attribute__((interrupt)) void segment_not_present_handler(struct interrupt_frame *frame)
{
    panic("SEGMENT NOT PRESENT", frame);
}

__attribute__((interrupt)) void stack_fault_handler(struct interrupt_frame *frame)
{
    panic("STACK FAULT", frame);
}

__attribute__((interrupt)) void floating_point_error_handler(struct interrupt_frame *frame)
{
    panic("FLOATING POINT ERROR", frame);
}

__attribute__((interrupt)) void alignment_check_handler(struct interrupt_frame *frame)
{
    panic("ALIGNMENT CHECK", frame);
}

__attribute__((interrupt)) void machine_check_handler(struct interrupt_frame *frame)
{
    panic("MACHINE CHECK", frame);
}

__attribute__((interrupt)) void simd_floating_point_handler(struct interrupt_frame *frame)
{
    panic("SIMD FLOATING POINT", frame);
}

__attribute__((interrupt)) void virtualization_exception_handler(struct interrupt_frame *frame)
{
    panic("VIRTUALIZATION EXCEPTION", frame);
}

__attribute__((interrupt)) void kb_handler(struct interrupt_frame *frame)
{
        uint8_t scancode = inb(0x60);

        printf("irq 1");

        pic_end_master();
}