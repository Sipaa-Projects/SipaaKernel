#ifndef GDT_H
#define GDT_H

#include <lib/stdtype.h>

typedef struct
{
    ui16 size;
    ui64 offset;
} __attribute__((packed)) gdt_pointer;

typedef struct {
    ui32 reserved0;
    ui64 rsp0;
    ui64 rsp1;
    ui64 rsp2;
    ui64 reserved1;
    ui64 ist1;
    ui64 ist2;
    ui64 ist3;
    ui64 ist4;
    ui64 ist5;
    ui64 ist6;
    ui64 ist7;
    ui64 reserved2;
    ui16 reserved3;
    ui16 iomap_base;
} __attribute__((packed)) tss_entry_t;

typedef struct
{
    ui16 limit0;
    ui16 base0;
    ui8 base1;
    ui8 access;
    ui8 limit1_flags;
    ui8 base2;
} __attribute__((packed)) gdt_entry;

int gdt_init(ui64 rsp0);
void set_kernel_stack(ui64 stack);

#endif