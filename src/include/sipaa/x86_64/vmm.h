/// @brief Credit to Astrido for the VMM. https://github.com/asterd-og/ZanOS

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <sipaa/pmm.h>
#include <sipaa/bootsrv.h>
#include <limine.h>

extern uint64_t *vmm_kernel_pml4;

#define PTE_PRESENT 1ull
#define PTE_WRITABLE (1ull << 1)
#define PTE_USER (1ull << 2)
#define PTE_NX (1ull << 63)

#define PTE_ADDR_MASK 0x000ffffffffff000
#define PTE_GET_ADDR(VALUE) ((VALUE) & PTE_ADDR_MASK)
#define PTE_GET_FLAGS(VALUE) ((VALUE) & ~PTE_ADDR_MASK)

#define VIRTUAL_TO_PHYSICAL(ptr) ((void*)((uint64_t)ptr) + BootSrv_GetHHDMOffset())
#define PHYSICAL_TO_VIRTUAL(ptr) ((void*)((uint64_t)ptr) - BootSrv_GetHHDMOffset())

typedef char SymbolT[];

extern SymbolT text_start_ld;
extern SymbolT text_end_ld;

extern SymbolT rodata_start_ld;
extern SymbolT rodata_end_ld;

extern SymbolT data_start_ld;
extern SymbolT data_end_ld;

void Vmm_Initialize();
uint64_t *Vmm_NewPML4();
void vmm_switch_pml4(uint64_t *pml4);
void vmm_map(uint64_t *pml4, uint64_t vaddr, uint64_t paddr, uint64_t flags);
void vmm_unmap(uint64_t *pml4, uint64_t vaddr);
void print_kernel_size();