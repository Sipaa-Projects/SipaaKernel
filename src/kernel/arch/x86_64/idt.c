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

    idt_set_entry(0, (uint64_t)zero_division_handler, 0x8E);
    idt_set_entry(1, (uint64_t)debug_handler, 0x8E);
    idt_set_entry(2, (uint64_t)non_maskable_interrupt_handler, 0x8E);
    idt_set_entry(3, (uint64_t)breakpoint_handler, 0x8E);
    idt_set_entry(4, (uint64_t)overflow_handler, 0x8E);
    idt_set_entry(5, (uint64_t)bound_range_exceeded_handler, 0x8E);
    idt_set_entry(6, (uint64_t)invalid_opcode_handler, 0x8E);
    idt_set_entry(7, (uint64_t)device_not_available_handler, 0x8E);
    idt_set_entry(8, (uint64_t)double_fault_handler, 0x8E);
    idt_set_entry(10, (uint64_t)invalid_tss_handler, 0x8E);
    idt_set_entry(11, (uint64_t)segment_not_present_handler, 0x8E);
    idt_set_entry(12, (uint64_t)stack_fault_handler, 0x8E);
    idt_set_entry(13, (uint64_t)gpf_handler, 0x8E);
    idt_set_entry(14, (uint64_t)pf_handler, 0x8E);
    idt_set_entry(16, (uint64_t)floating_point_error_handler, 0x8E);
    idt_set_entry(17, (uint64_t)alignment_check_handler, 0x8E);
    idt_set_entry(18, (uint64_t)machine_check_handler, 0x8E);
    idt_set_entry(19, (uint64_t)simd_floating_point_handler, 0x8E);
    idt_set_entry(20, (uint64_t)virtualization_exception_handler, 0x8E);
    // idt_set_entry(0x80, (uint64_t)syscall_handler, 0x8E);

    idt_set_entry(33, (uint64_t)kb_handler, 0x8E);

    remap_pic();
    outb(PIC1_DATA, 0b11111101);
    outb(PIC2_DATA, 0b11111111);

    __asm__ volatile("lidt %0"
                     :
                     : "m"(idtr));
    __asm__ volatile("sti");
}