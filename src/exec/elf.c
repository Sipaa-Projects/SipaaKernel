#ifdef __x86_64__

#include <sipaa/libc/types.h>
#include <sipaa/libc/string.h>
#include <sipaa/exec/elf.h>
#include <sipaa/logger.h>
#include <sipaa/pmm.h>
#include <sipaa/x86_64/vmm.h>

char *Elf64_GetTargetABI(Elf64_Ehdr *ehdr)
{
    switch (ehdr->e_ident[EI_OSABI]) // ELF specification: Identification index 7 is the ABI.
    {
    case ELFOSABI_SYSV: // UNIX SysV
        return "UNIX System V";
        break;
    
    case ELFOSABI_HPUX: // HP UX
        return "HP-UX";
        break;

    case ELFOSABI_NETBSD: // NetBSD
        return "NetBSD (Berkeley Software Distrib.)";
        break;

    case ELFOSABI_LINUX: // Linux
        return "Linux";
        break;

    case ELFOSABI_SOLARIS: // Solaris
        return "SUN Solaris";
        break;

    case ELFOSABI_AIX: // IBM ABX
        return "IBM AIX";
        break;

    case ELFOSABI_IRIX: // SGI Irix
        return "SGI Irix";
        break;

    case ELFOSABI_FREEBSD: // FreeBSD
        return "FreeBSD (Berkeley Software Distrib.)";
        break;

    case ELFOSABI_TRU64: // Compaq TRU64 UNIX
        return "Compaq TRU64";
        break;

    case ELFOSABI_MODESTO: // Novell Modesto
        return "Novell Modesto";
        break;

    case ELFOSABI_OPENBSD: // OpenBSD
        return "OpenBSD (Berkeley Software Distrib.)";
        break;

    case ELFOSABI_SIPAAKERNEL: // SipaaKernel
        return "SipaaKernel";
        break;

    case EM_ARM: // ARM
        return "ARM";
        break;

    case ELFOSABI_STANDALONE: // Standalone
        return "Standalone";
        break;
    default:
        break;
    }
}

bool Elf64_DoChecks(Elf64_Ehdr *ehdr)
{
    if (ehdr->e_ident[0] != 0x7f || ehdr->e_ident[1] != 'E' ||
        ehdr->e_ident[2] != 'L' || ehdr->e_ident[3] != 'F')
    {
        Log(LT_ERROR, "ElfLoad", "Not a valid ELF file.\n");
        return false;
    }

    Log(LT_INFO, "ElfLoad", "EHDR: e_type     : %d\n", ehdr->e_type);
    Log(LT_INFO, "ElfLoad", "      e_machine  : %d\n", ehdr->e_machine);
    Log(LT_INFO, "ElfLoad", "      e_version  : %d\n", ehdr->e_version);
    Log(LT_INFO, "ElfLoad", "      e_entry    : %p\n", ehdr->e_entry);
    Log(LT_INFO, "ElfLoad", "      e_phoff    : 0x%x\n", ehdr->e_phoff);
    Log(LT_INFO, "ElfLoad", "      e_shoff    : 0x%x\n", ehdr->e_shoff);
    Log(LT_INFO, "ElfLoad", "      e_flags    : %d\n", ehdr->e_flags);
    Log(LT_INFO, "ElfLoad", "      e_ehsize   : %d\n", ehdr->e_ehsize);
    Log(LT_INFO, "ElfLoad", "      e_phentsize: %d\n", ehdr->e_phentsize);
    Log(LT_INFO, "ElfLoad", "      e_phnum    : %d\n", ehdr->e_phnum);
    Log(LT_INFO, "ElfLoad", "      e_shentsize: %d\n", ehdr->e_shentsize);
    Log(LT_INFO, "ElfLoad", "      e_shnum    : %d\n", ehdr->e_shnum);
    Log(LT_INFO, "ElfLoad", "      e_shstrndx : %d\n", ehdr->e_shstrndx);
    Log(LT_INFO, "ElfLoad", "Additional infos:\n");
    Log(LT_INFO, "ElfLoad", "      Target ABI : %s\n", Elf64_GetTargetABI(ehdr));

    if (ehdr->e_type != ET_EXEC)
    {
        Log(LT_ERROR, "ElfLoad", "Not an executable ELF file.\n");
        return false;
    }
    
    if (ehdr->e_machine != EM_X86_64)
    {
        Log(LT_ERROR, "ElfLoad", "Not a x86_64 ELF file! Do you want to break your PC?\n");
        return false;
    }

    if (ehdr->e_ident[7] != ELFOSABI_SYSV && ehdr->e_ident[7] != ELFOSABI_LINUX && ehdr->e_ident[7] != ELFOSABI_SIPAAKERNEL)
    {
        Log(LT_ERROR, "ElfLoad", "This ELF file does not target a supported ABI (UNIX System V, Linux or SipaaKernel)\n");
        return false;
    }

    return true;
}

void Elf64_DumpSymbols(const char *img, Elf64_Ehdr *ehdr)
{
    Elf64_Shdr *shdr = (Elf64_Shdr *)(img + ehdr->e_shoff);
    Elf64_Sym *symtab = NULL;
    int symnum = 0;

    const char *shstrtab = img + shdr[ehdr->e_shstrndx].sh_offset;
    const char *strtab = NULL;

    Log(LT_INFO, "ElfLoad", "Sections:\n");
    for (int j = 0; j < ehdr->e_shnum; j++) {
        Log(LT_INFO, "ElfLoad", "[%2d] %s: %d\n", j, shstrtab + shdr[j].sh_name, shdr[j].sh_type);
        if (shdr[j].sh_type == SHT_SYMTAB)
        {
            symtab = (Elf64_Sym *)(img + shdr[j].sh_offset);
            symnum = shdr[j].sh_size / shdr[j].sh_entsize;
        }
        else if (shdr[j].sh_type == SHT_STRTAB && CompareStrings(shstrtab + shdr[j].sh_name, ".strtab") == 0)
        {
            strtab = img + shdr[j].sh_offset;
        }
    }

    Log(LT_INFO, "ElfLoad", "\n");
    Log(LT_INFO, "ElfLoad", "Symbols:\n");

    for (int k = 0; k < symnum; k++)
    {
        Log(LT_INFO, "ElfLoad", "[%d] %p: %s\n", k, symtab[k].st_value, strtab + symtab[k].st_name);
    }
}

uint64_t Elf64_Load(const char *img)
{
    Elf64_Ehdr *hdr = (Elf64_Ehdr *)img;

    if (!Elf64_DoChecks(hdr))
        return 0;

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

    Elf64_DumpSymbols(img, hdr);

    Log(LT_SUCCESS, "ElfLoad", "Loaded ELF file. Entry: %p\n", hdr->e_entry);
    return hdr->e_entry;
}

#endif