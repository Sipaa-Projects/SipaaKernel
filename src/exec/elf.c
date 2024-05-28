#include <sipaa/libc/types.h>
#include <sipaa/exec/elf.h>
#include <sipaa/logger.h>
#include <sipaa/pmm.h>
#include <sipaa/x86_64/vmm.h>

uint64_t Elf64_Load(const char *img)
{
    Elf64_Ehdr *hdr = (Elf64_Ehdr *)img;

    if (hdr->e_ident[0] != 0x7f || hdr->e_ident[1] != 'E' ||
        hdr->e_ident[2] != 'L' || hdr->e_ident[3] != 'F')
    {
        Log(LT_ERROR, "ElfLoad", "Not a valid ELF file.\n");
        return -1;
    }

    if (hdr->e_type != 2)
    {
        Log(LT_ERROR, "ElfLoad", "Not an executable ELF file.\n");
        return -1;
    }

    Elf64_Phdr *phdr = (Elf64_Phdr *)(img + hdr->e_phoff);

    for (uint16_t i = 0; i < hdr->e_phnum; i++, phdr++)
    {
        if (phdr->p_type == 1)
        {
            uintptr_t seg_start = ALIGN_DOWN(phdr->p_vaddr, PAGE_SIZE);
            uintptr_t seg_end = ALIGN_UP(seg_start + phdr->p_memsz, PAGE_SIZE);
            void *seg = Pmm_Allocate(DIV_ROUND_UP(phdr->p_memsz, PAGE_SIZE));

            memcpy((void *)phdr->p_vaddr, (void *)img + phdr->p_offset, phdr->p_filesz);
            memset((void *)(phdr->p_vaddr + phdr->p_filesz), 0, phdr->p_memsz - phdr->p_filesz);

            for (uintptr_t addr = seg_start; addr < seg_end; addr += PAGE_SIZE)
            {
                Vmm_Map(vmm_kernel_address_space, addr, addr - seg_start + (uintptr_t)seg, PTE_PRESENT | PTE_WRITABLE);
            }
        }
    }

    Log(LT_SUCCESS, "ElfLoad", "Loaded ELF file. Entry: %p\n", hdr->e_entry);
    return hdr->e_entry;
}