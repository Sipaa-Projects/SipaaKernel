#pragma once

#include <stdbool.h>
#include <stdint.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

extern uint64_t pit_ticks;

typedef struct IdtEntry
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t types_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed)) IdtEntryT;

typedef struct Registers
{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rbp;
    uint64_t rbx;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rax;
    uint64_t int_no;
    uint64_t err_code;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed)) RegistersT;

extern uint64_t isr0;
extern uint64_t isr1;
extern uint64_t isr2;
extern uint64_t isr3;
extern uint64_t isr4;
extern uint64_t isr5;
extern uint64_t isr6;
extern uint64_t isr7;
extern uint64_t isr8;
extern uint64_t isr9;
extern uint64_t isr10;
extern uint64_t isr11;
extern uint64_t isr12;
extern uint64_t isr13;
extern uint64_t isr14;
extern uint64_t isr16;
extern uint64_t isr17;
extern uint64_t isr18;
extern uint64_t isr19;
extern uint64_t isr20;
extern uint64_t isr21;
extern uint64_t isr22;
extern uint64_t isr23;
extern uint64_t isr24;
extern uint64_t isr25;
extern uint64_t isr26;
extern uint64_t isr27;
extern uint64_t isr28;
extern uint64_t isr29;
extern uint64_t isr30;
extern uint64_t isr31;
extern uint64_t isr32;
extern uint64_t isr33;

extern uint64_t CR2_Read();

void Idt_Initialize();