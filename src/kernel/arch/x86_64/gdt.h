#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include <video/video.h>
#include <arch/io.h>

typedef struct {
    uint16_t limit0;
    uint16_t base0;
    uint8_t base1;
    uint8_t access;
    uint8_t limit1_flags;
    uint8_t base2;
} __attribute__((packed)) gdt_entry;

typedef struct {
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
} __attribute__((packed)) tss_entry_t;

typedef struct {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed)) gdt_pointer;

void init_gdt(uint64_t kernel_rsp);
void set_kernel_stack(void* stack);

#endif
