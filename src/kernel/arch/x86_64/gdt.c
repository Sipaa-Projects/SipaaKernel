#include "gdt.h"

gdt_entry gdt[0xff];
gdt_pointer gdt_ptr;
tss_entry_t tss_entry;

extern void load_gdt(gdt_pointer *);
extern void load_tss(void);

void set_gdt_entry(int i, uint16_t limit, uint16_t base, uint8_t access, uint8_t flags)
{
    gdt[i].base0 = base & 0xFFFF;
    gdt[i].base1 = (base >> 16) & 0xFF;
    gdt[i].base2 = (base >> 24) & 0xFF;
    gdt[i].limit0 = limit & 0xFFFF;
    gdt[i].access = access;
    gdt[i].limit1_flags = ((limit >> 16) & 0x0F) | (flags & 0xF0);
}

void set_tss_gate(int32_t num, uint64_t base, uint32_t limit)
{
    gdt[num].base0 = base & 0xFFFF;
    gdt[num].base1 = (base >> 16) & 0xFF;
    gdt[num].base2 = (base >> 24) & 0xFF;
    gdt[num].limit0 = limit & 0xFFFF;
    gdt[num].access = 0x89;
    gdt[num].limit1_flags = ((limit >> 16) & 0x0F) | 0x00;
}

static void init_tss(uint64_t rsp0)
{
    memset(&tss_entry, 0, sizeof(tss_entry_t));
    tss_entry.rsp0 = rsp0;
}

void init_gdt(uint64_t kernel_rsp)
{
    gdt_ptr.size = sizeof(gdt) - 1;
    gdt_ptr.offset = (uint64_t)&gdt;

    // 0: NULL
    set_gdt_entry(0, 0, 0, 0, 0);

    // 1: Kernel Code Segment
    set_gdt_entry(1, 0xFFFFF, 0, 0x9A, 0xAF); // exec/read, ring 0

    // 2: Kernel Data Segment
    set_gdt_entry(2, 0xFFFFF, 0, 0x92, 0xAF); // read/write, ring 0

    // 3: NULL
    set_gdt_entry(3, 0, 0, 0, 0);

    // 4: User Code Segment
    set_gdt_entry(4, 0xFFFFF, 0, 0xFA, 0xAF); // exec/read, ring 3

    // 5: User Data Segment
    set_gdt_entry(5, 0xFFFFF, 0, 0xF2, 0xAF); // read/write, ring 3

    set_tss_gate(6, (uint64_t)&tss_entry, sizeof(tss_entry_t));

    init_tss(kernel_rsp);
    load_gdt(&gdt_ptr);
    load_tss();
}

void set_kernel_stack(void *stack)
{
    tss_entry.rsp0 = (uint64_t)stack;
}
