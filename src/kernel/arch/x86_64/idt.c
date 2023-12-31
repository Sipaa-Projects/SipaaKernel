// SKB_X86_64_ONLY

#include "idt.h"
#include <logger/logger.h>

__attribute__((aligned(0x10))) static idt_entry idt[256];
static idt_pointer idtr;

void _panic(char *message, struct interrupt_frame *frame)
{
    log(LT_ERROR, "kernel", "pnc: %s\n", message);
    log(LT_ERROR, "kernel", "rax: %llu, rbx: %llu, rcx: %llu, rdx: %llu\n", frame->rax, frame->rbx, frame->rcx, frame->rdx);
    log(LT_ERROR, "kernel", "int: %llu\n", frame->int_num);
    log(LT_ERROR, "kernel", "cpu halted. goodbye.\n");
    //printf("[kernel] pnc: %s\n", message);
    while (1)
    {
        __asm__("hlt");
    }
}


void idt_set_entry(uint8_t vector, void *isr, uint8_t flags)
{
    idt_entry *descriptor = &idt[vector];

    descriptor->base_low = (uint64_t)isr & 0xFFFF;
    descriptor->cs = 0x08;
    descriptor->ist = 0;
    descriptor->attributes = flags;
    descriptor->base_mid = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->base_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->rsv0 = 0;
}

void init_idt()
{
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry) * 256 - 1;

    idt_set_entry(0, zero_division_handler, 0x8E);
    idt_set_entry(1, debug_handler, 0x8E);
    idt_set_entry(2, non_maskable_interrupt_handler, 0x8E);
    idt_set_entry(3, breakpoint_handler, 0x8E);
    idt_set_entry(4, overflow_handler, 0x8E);
    idt_set_entry(5, bound_range_exceeded_handler, 0x8E);
    idt_set_entry(6, invalid_opcode_handler, 0x8E);
    idt_set_entry(7, device_not_available_handler, 0x8E);
    idt_set_entry(8, double_fault_handler, 0x8E);
    idt_set_entry(10, invalid_tss_handler, 0x8E);
    idt_set_entry(11, segment_not_present_handler, 0x8E);
    idt_set_entry(12, stack_fault_handler, 0x8E);
    idt_set_entry(13, gpf_handler, 0x8E);
    idt_set_entry(14, pf_handler, 0x8E);
    idt_set_entry(16, floating_point_error_handler, 0x8E);
    idt_set_entry(17, alignment_check_handler, 0x8E);
    idt_set_entry(18, machine_check_handler, 0x8E);
    idt_set_entry(19, simd_floating_point_handler, 0x8E);
    idt_set_entry(20, virtualization_exception_handler, 0x8E);
    // idt_set_entry(0x80, (uint64_t)syscall_handler, 0x8E);

    idt_set_entry(33, kb_handler, 0x8E);

    remap_pic();
    outb(PIC1_DATA, 0b11111101);
    outb(PIC2_DATA, 0b11111111);

    __asm__ volatile("lidt %0"
                     :
                     : "m"(idtr));
    __asm__ volatile("sti");
}