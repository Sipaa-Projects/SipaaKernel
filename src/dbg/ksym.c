#include <sipaa/ksym.h>
#include <sipaa/bootsrv.h>
#include <sipaa/libc/string.h>
#include <sipaa/logger.h>
#include <sipaa/pmm.h>

Elf64_Sym *KernelSymbols_SymTable;
char *KernelSymbols_StrTable = NULL;
int KernelSymbols_SymCount = 0;

int *KernelSymbols_IpBindingTable;
int KernelSymbols_InstructionCount;

void KernelSymbols_Initialize()
{
    const char *img = (const char *)BootSrv_GetModule(-2)->address; // Module -2 will make BootSrv returns the kernel image.
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)img;

    if (ehdr->e_ident[0] != 0x7f || ehdr->e_ident[1] != 'E' ||
        ehdr->e_ident[2] != 'L' || ehdr->e_ident[3] != 'F')
    {
        // sipaakernel now try to get why he booted :skull:
        Log(LT_FATAL, "SipaaKernel", "How did i even boot? I'm not a ELF file! *starts thinking forever*\n");

        #ifdef __x86_64__
        asm("cli");
        #endif
        while (true)
        #ifdef __x86_64__
            asm("hlt");
        #else
            asm("wfi");
        #endif
    }

    Elf64_Shdr *shdr = (Elf64_Shdr *)(img + ehdr->e_shoff);
    const char *shstrtab = img + shdr[ehdr->e_shstrndx].sh_offset;

    for (int j = 0; j < ehdr->e_shnum; j++) {
        if (shdr[j].sh_type == SHT_SYMTAB)
        {
            KernelSymbols_SymTable = (Elf64_Sym *)(img + shdr[j].sh_offset);
            KernelSymbols_SymCount = shdr[j].sh_size / shdr[j].sh_entsize;
        }
        else if (shdr[j].sh_type == SHT_STRTAB && CompareStrings(shstrtab + shdr[j].sh_name, ".strtab") == 0)
        {
            KernelSymbols_StrTable = img + shdr[j].sh_offset;
        }
    }

    // Try getting a symbol
    Elf64_Sym *sym = KernelSymbols_GetFromIP(0xffffffff8000a6d9);
    if (sym)
    {
        Log(LT_INFO, "ksymbols", "got symbol %s from 0xffffffff8000a6d9\n", KernelSymbols_StrTable + sym->st_name);
    }
    else
    {
        Log(LT_ERROR, "ksymbols", "i didn't get any symbol :-(\n");
    }
}

char *KernelSymbols_GetSymbolName(uint32_t offset)
{
    return KernelSymbols_StrTable + offset;
}

Elf64_Sym *KernelSymbols_GetFromIP(uint64_t ip)
{
    // not the most efficient, but works atleast.
    for (int i = 0; i < KernelSymbols_SymCount; i++)
    {
        uint64_t value = KernelSymbols_SymTable[i].st_value;
        uint64_t size = KernelSymbols_SymTable[i].st_size;

        for (uint64_t j = 0; j < size; j++, value++)
        {
            if (value == ip)
            {
                return &KernelSymbols_SymTable[i];
            }
        }
    }

    return NULL;
}
