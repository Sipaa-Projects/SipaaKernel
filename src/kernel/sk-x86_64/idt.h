#ifndef __IDT_H
#define __IDT_H

#include <stdint.h>
#include <stdbool.h>

struct x86_64_registers
{
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
    uint64_t int_num, error_code;
    uint64_t rip, cs, rflags, rsp, ss;
};

struct idt_entry {
  uint16_t offset_low;
  uint16_t selector; 
  uint8_t ist;      
  uint8_t type_attr;
  uint16_t offset_middle;
  uint32_t offset_high;
  uint32_t zero; 
} __attribute__((packed));

struct idt_ptr {
  uint16_t limit;
  uint64_t base; 
} __attribute__((packed));

int idt_init();
void idt_set_entry(int num, void* handler, uint8_t ist);

#endif