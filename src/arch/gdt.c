/**
 * Global Descriptor Table(GDT) setup
 */
#include <sipaa/i686/gdt.h>

GDT g_gdt[NO_GDT_DESCRIPTORS];
GDT_PTR g_gdt_ptr;

// asm gdt functions, define in load_gdt.asm
extern void load_gdt(uint32_t gdt_ptr);

/**
 * fill entries of GDT 
 */
void Gdt_SetEntry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    GDT *this = &g_gdt[index];

    this->segment_limit = limit & 0xFFFF;
    this->base_low = base & 0xFFFF;
    this->base_middle = (base >> 16) & 0xFF;
    this->access = access;

    this->granularity = (limit >> 16) & 0x0F;
    this->granularity = this->granularity | (gran & 0xF0);

    this->base_high = (base >> 24 & 0xFF);
}

// initialize GDT
void Gdt_Initialize() {
    g_gdt_ptr.limit = sizeof(g_gdt) - 1;
    g_gdt_ptr.base_address = (uint32_t)g_gdt;

    // NULL segment
    Gdt_SetEntry(0, 0, 0, 0, 0);
    // code segment
    Gdt_SetEntry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // data segment
    Gdt_SetEntry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    // user code segment
    Gdt_SetEntry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    // user data segment
    Gdt_SetEntry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    load_gdt((uint32_t)&g_gdt_ptr);
}