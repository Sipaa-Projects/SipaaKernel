/*!
 * @file
 * @brief x86-64 IDT implementation.
 * 
 * The Interrupt Descriptor Table (IDT) is a binary data structure specific to the IA-32 and x86-64 architectures. 
 * It is the Protected Mode and Long Mode counterpart to the Real Mode Interrupt Vector Table (IVT) telling the CPU where the Interrupt Service Routines (ISR) are located (one per interrupt vector).
 */

// #include <framebuffer/font.h>
#include "idt.h"
#include "pic.h"
#include <sk-logger/logger.h>
#include <stdint.h>
#include <stddef.h>

struct idt_entry idt[256]; 
struct idt_ptr idtp;

/// @brief Internal architecture panic (TODO: replace it with kernel panic)
/// @param regs Registers
/// @param message The message to show
__attribute__((no_caller_saved_registers)) void __internal_arch_panic(struct x86_64_registers *regs, char *message) {
    log(LT_ERROR, "%s\n", message);
    log(LT_ERROR, "CPU Registers: \n");
    log(LT_ERROR, "rax: %llu, rbx: %llu\n", regs->rax, regs->rbx);
    log(LT_ERROR, "int: %llu, rip: %llu\n", regs->int_num, regs->rip);
    while (1) { asm("hlt"); }
}

__attribute__((interrupt)) void div_zero_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Division by 0"); }
__attribute__((interrupt)) void debug_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Debug"); }
__attribute__((interrupt)) void nmi_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Non-Maskable Interrupt"); }
__attribute__((interrupt)) void bkp_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Breakpoint"); }
__attribute__((interrupt)) void ofw_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Overflow"); }
__attribute__((interrupt)) void bre_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Bound Rance Exceeded"); }
__attribute__((interrupt)) void iop_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Invalid opcode"); }
__attribute__((interrupt)) void dna_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Device not available"); }
__attribute__((interrupt)) void dfa_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Double fault"); }
__attribute__((interrupt)) void itss_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Invalid TSS"); }
__attribute__((interrupt)) void snp_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Segment not present"); }
__attribute__((interrupt)) void ssf_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Stack-Segment Fault"); }
__attribute__((interrupt)) void gpf_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "General Protection Fault"); }
__attribute__((interrupt)) void pfa_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Page Fault"); }
__attribute__((interrupt)) void x87fpo_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "x87 Floating Point"); }
__attribute__((interrupt)) void ach_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Alignment Check"); }
__attribute__((interrupt)) void mch_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Machine Check"); }
__attribute__((interrupt)) void simdfpo_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "SIMD Floating Point"); }
__attribute__((interrupt)) void virtexc_handler(struct x86_64_registers *regs) { __internal_arch_panic(regs, "Virtualization Exception"); }

__attribute__((interrupt)) void irq0_handler(struct x86_64_registers *regs) {
    // Handle IRQ0  
    (void)regs;
}

__attribute__((interrupt)) void irq1_handler(struct x86_64_registers *regs) {
    (void)regs;
    if (inb(0x60) & 0x80)
    {
        pic_end_master();
        return;
    }
    log(LT_INFO, "Keyboard IRQ has been raised!\n");
    pic_end_master();
} 

void idt_load() {
    remap_pic();
    outb(PIC1_DATA, 0b11111101);
    outb(PIC2_DATA, 0b11111111);

    // Load to CPU
    log(LT_INFO, "Loading IDT into CPU...\n");
    __asm__ volatile("lidt %0" : : "m"(idtp)); 
}

/// @brief Set IDT entry
/// @param num Vector
/// @param handler Handler function
/// @param ist IST
void idt_set_entry(int num, void* handler, uint8_t ist) {
    struct idt_entry* entry = &idt[num];

    entry->offset_low = (uint64_t)handler & 0xFFFF;
    entry->selector = 0x08; // Kernel CS
    entry->ist = ist;
    entry->type_attr = 0x8E; // Interrupt gate, present
    entry->offset_middle = ((uint64_t)handler >> 16) & 0xFFFF;  
    entry->offset_high = ((uint64_t)handler >> 32) & 0xFFFFFFFF;
    entry->zero = 0;
}

/// @brief Init IDT
/// @return 0 if success
int idt_init() {
    log(LT_INFO, "Setting up IDT pointer...\n");
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint64_t)&idt;
    // Exception handlers  
    log(LT_INFO, "Setting up CPU exceptions entries...\n");
    idt_set_entry(0, div_zero_handler, 0);
    idt_set_entry(1, debug_handler, 0);
    idt_set_entry(2, nmi_handler, 0);
    idt_set_entry(3, bkp_handler, 0);
    idt_set_entry(4, ofw_handler, 0);
    idt_set_entry(5, bre_handler, 0);
    idt_set_entry(6, iop_handler, 0);
    idt_set_entry(7, dna_handler, 0);
    idt_set_entry(8, dfa_handler, 0);
    idt_set_entry(10, itss_handler, 0);
    idt_set_entry(11, snp_handler, 0);
    idt_set_entry(12, ssf_handler, 0);
    idt_set_entry(13, gpf_handler, 0);
    idt_set_entry(14, pfa_handler, 0);
    idt_set_entry(16, x87fpo_handler, 0);
    idt_set_entry(17, ach_handler, 0);
    idt_set_entry(18, mch_handler, 0);
    idt_set_entry(19, simdfpo_handler, 0);
    idt_set_entry(20, virtexc_handler, 0);

    // IRQ handlers
    log(LT_INFO, "Setting up IRQ exceptions entries...\n");
    idt_set_entry(0x20, irq0_handler, 0);
    idt_set_entry(0x21, irq1_handler, 0);

    idt_load();
    asm("sti");
    return 0;
}