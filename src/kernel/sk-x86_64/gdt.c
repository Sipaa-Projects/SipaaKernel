/*!
 * @file
 * @brief x86-64 GDT implementation.
 * 
 * The Global Descriptor Table (GDT) is a binary data structure specific to the IA-32 and x86-64 architectures.
 * It contains entries telling the CPU about memory segments.
 */

#include "gdt.h"
#include <sk-logger/logger.h>

gdt_entry gdt[0xFF];
gdt_pointer gdtr;
tss_entry_t tss_entry;

/// @brief External pointer to the 'load_gdt' function defined in 'gdt.asm'
/// @param gdtptr The GDT pointer
extern void load_gdt(gdt_pointer* gdtptr);

/// @brief External pointer to the 'load_tss' function defined in 'gdt.asm'
extern void load_tss();

/// @brief Set a GDT entry
/// @param i The entry index
/// @param limit The limit
/// @param base The base
/// @param access The access
/// @param flags The flags
void set_gdt_entry(int i, uint16_t limit, uint16_t base, uint8_t access, uint8_t flags) {
    gdt[i].base0 = base & 0xFFFF;
    gdt[i].base1 = (base >> 16) & 0xFF;
    gdt[i].base2 = (base >> 24) & 0xFF;
    gdt[i].limit0 = limit & 0xFFFF;
    gdt[i].access = access;
    gdt[i].limit1_flags = ((limit >> 16) & 0x0F) | (flags & 0xF0);
}

/// @brief Set a TSS gate
/// @param i The entry index in GDT
/// @param base The base
/// @param limit The limit
void set_tss_gate(int i, uint64_t base, uint32_t limit)
{
    gdt[i].base0 = base & 0xFFFF;
    gdt[i].base1 = (base >> 16) & 0xFF;
    gdt[i].base2 = (base >> 24) & 0xFF;
    gdt[i].limit0 = limit & 0xFFFF;
    gdt[i].access = 0xE9;
    gdt[i].limit1_flags = ((limit >> 16) & 0x0F) | 0x00;
}

/// @brief Init the TSS
/// @param rsp0 Kernel stack
void init_tss(uint64_t rsp0)
{
    char *ptr_c = (char *)&tss_entry;
    for (int i = 0; i < (int)sizeof(tss_entry_t); i++)
    {
        ptr_c[i] = (char)0;
    }
    tss_entry.rsp0 = rsp0;
}

/// @brief Init the GDT & TSS
/// @param rsp0 The kernel stack
/// @return 0 if success
int gdt_init(uint64_t rsp0) {
    log(LT_INFO, "setting up GDT entries...\n");
    set_gdt_entry(0, 0, 0, 0, 0);
    set_gdt_entry(1, (uint16_t)0xFFFFFF, 0, 0x9A, 0xAF);
    set_gdt_entry(2, (uint16_t)0xFFFFFF, 0, 0x92, 0xAF);
    set_gdt_entry(3, (uint16_t)0xFFFFFF, 0, 0x96, 0xAF);
    set_gdt_entry(4, (uint16_t)0xFFFFFF, 0, 0xFA, 0xAF);
    set_gdt_entry(5, (uint16_t)0xFFFFFF, 0, 0xF2, 0xAF);
    set_gdt_entry(6, (uint16_t)0xFFFFFF, 0, 0xF6, 0xAF);
    set_tss_gate(7, (uint64_t)&tss_entry, sizeof(tss_entry_t));

    log(LT_INFO, "setting up GDT pointer...\n");
    gdtr.size = sizeof(gdt) - 1;
    gdtr.offset = (uint64_t)&gdt;

    log(LT_INFO, "loading GDT/TSS into your computer...\n");
    init_tss(rsp0);
    load_gdt(&gdtr);
    load_tss();

    return 0;
}

/// @brief Set the kernel stack
/// @param stack The new kernel stack
void set_kernel_stack(uint64_t stack)
{
    tss_entry.rsp0 = stack;
}