#pragma once

#include <cstdint>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

namespace Sk {
namespace Arch {

struct Registers64
{
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
    uint64_t int_num, error_code;
    uint64_t rip, cs, rflags, rsp, ss;
};

typedef struct
{
    uint16_t base_low;
    uint16_t cs;
    uint8_t ist;
    uint8_t attributes;
    uint16_t base_mid;
    uint32_t base_high;
    uint32_t rsv0;
} __attribute__((packed)) IdtEntry;

typedef struct
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) IdtPtr;

class Idt {
public:
    __attribute__((aligned(0x10))) static IdtEntry entries[256];
    static IdtPtr idtr;

    static void PicRemap();
    static void PicEoi(uint8_t irq);
    static void SetEntry(uint8_t vector, void *isr, uint8_t flags);
    static void Init();
};

}
}