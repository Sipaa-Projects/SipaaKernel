#pragma once

#include <cstdint>

namespace Sk {
namespace Arch {

typedef struct {
    uint16_t limit0;
    uint16_t base0;
    uint8_t base1;
    uint8_t access;
    uint8_t limit1_flags;
    uint8_t base2;
} __attribute__((packed)) GdtEntry;

struct TssEntry {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iomap_base;
} ;

typedef struct {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed)) GdtPtr;

class Gdt {
public:
    static GdtEntry entries[0xFF];
    static GdtPtr ptr;
    static TssEntry tssentry;

    static void InitTss(uint64_t kernel_stack);
    static void SetEntry(int i, uint16_t limit, uint16_t base, uint8_t access, uint8_t flags);
    static void SetTssGate(int32_t num, uint64_t base, uint32_t limit);
    static void SetKernelStack(uint64_t stack);
    static int Init(uint64_t kernel_stack);
};

} // namespace Arch
} // namespace Sk
