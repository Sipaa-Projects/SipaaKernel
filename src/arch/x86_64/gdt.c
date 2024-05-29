#ifdef __x86_64__

#include <sipaa/x86_64/gdt.h>
#include <sipaa/logger.h>
#include <sipaa/memory.h>

GdtEntryT gdt[0xff];
GdtPointerT gdt_ptr;
TssEntryT tss_entry;

extern void Gdt_Load(GdtPointerT *);
extern void Tss_Load(void);

void Gdt_SetEntry(int i, uint16_t limit, uint16_t base, uint8_t access, uint8_t flags)
{
    gdt[i].base0 = base & 0xFFFF;
    gdt[i].base1 = (base >> 16) & 0xFF;
    gdt[i].base2 = (base >> 24) & 0xFF;
    gdt[i].limit0 = limit & 0xFFFF;
    gdt[i].access = access;
    gdt[i].limit1_flags = ((limit >> 16) & 0x0F) | (flags & 0xF0);
}

void Tss_SetGate(int32_t num, uint64_t base, uint32_t limit)
{
    gdt[num].base0 = base & 0xFFFF;
    gdt[num].base1 = (base >> 16) & 0xFF;
    gdt[num].base2 = (base >> 24) & 0xFF;
    gdt[num].limit0 = limit & 0xFFFF;
    gdt[num].access = 0x89;
    gdt[num].limit1_flags = ((limit >> 16) & 0x0F) | 0x00;
}

static void Tss_Initialize(uint64_t rsp0)
{
    memset(&tss_entry, 0, sizeof(TssEntryT));
    tss_entry.rsp0 = rsp0;
}

void Gdt_Initialize(uint64_t kernel_rsp[])
{
    gdt_ptr.size = sizeof(gdt) - 1;
    gdt_ptr.offset = (uint64_t)&gdt;

    // 0: NULL
    Gdt_SetEntry(0, 0, 0, 0, 0);

    // 1: Kernel Code Segment
    Gdt_SetEntry(1, 0xFFFFF, 0, 0x9A, 0xAF); // exec/read, ring 0

    // 2: Kernel Data Segment
    Gdt_SetEntry(2, 0xFFFFF, 0, 0x92, 0xAF); // read/write, ring 0

    // 3: NULL
    Gdt_SetEntry(3, 0, 0, 0, 0);

    // 4: User Code Segment
    Gdt_SetEntry(4, 0xFFFFF, 0, 0xFA, 0xAF); // exec/read, ring 3

    // 5: User Data Segment
    Gdt_SetEntry(5, 0xFFFFF, 0, 0xF2, 0xAF); // read/write, ring 3

    Tss_SetGate(6, (uint64_t)&tss_entry, sizeof(TssEntryT));

    Tss_Initialize((uint64_t)kernel_rsp);
    Gdt_Load(&gdt_ptr);
    Tss_Load();
}

void Gdt_SetKernelStack(void *stack)
{
    tss_entry.rsp0 = (uint64_t)stack;
}

#endif