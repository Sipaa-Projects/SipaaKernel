#include <sipaa/bootsrv.h>
#include <sipaa/logger.h>
#include <stddef.h>

#if defined(__i686__)
#define KARCHITECTURE "x86 (32-bit)"
#elif defined(__x86_64__)
#define KARCHITECTURE "x86-64 (64-bit)"
#elif defined(__arm__)
#define KARCHITECTURE "ARM"
#elif defined(__aarch64__)
#define KARCHITECTURE "AArch64"
#elif defined(__PPC__)
#define KARCHITECTURE "PowerPC"
#elif defined(__PPC64__)
#define KARCHITECTURE "PowerPC64"
#else
#define KARCHITECTURE "Unknown Architecture"
#endif

// Please, for the love of Limine, don't optimize these.

static volatile struct limine_rsdp_request rsdp_request = {
  .id = LIMINE_RSDP_REQUEST,
  .revision = 0
};

static volatile struct limine_bootloader_info_request bir = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0
};

static volatile struct limine_kernel_file_request kfr = {
    .id = LIMINE_KERNEL_FILE_REQUEST,
    .revision = 0
};

static volatile struct limine_kernel_address_request kar = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};

static volatile struct limine_efi_system_table_request estr = {
    .id = LIMINE_EFI_SYSTEM_TABLE_REQUEST,
    .revision = 0
};

static volatile struct limine_framebuffer_request fbr = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

static volatile struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 0
};

static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 1
};

static volatile struct limine_paging_mode_request paging_mode_request = {
    .id = LIMINE_PAGING_MODE_REQUEST,
    .revision = 0,
    .response = NULL,
#ifdef __x86_64__
    .mode = LIMINE_PAGING_MODE_X86_64_4LVL,
#else
    .mode = LIMINE_PAGING_MODE_DEFAULT,
#endif
    .flags = 0
};

static volatile struct limine_smp_request _smp_request = {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0, 
    .response = NULL
};

struct limine_rsdp_response *BootSrv_GetAcpiRSDP()
{
    return rsdp_request.response;
}

struct limine_bootloader_info_response *BootSrv_GetBootloaderInfo()
{
    return bir.response;
}

uint64_t BootSrv_GetHHDMOffset()
{
    return hhdm_request.response->offset;
}

void BootSrv_EnumerateProtocolInfos()
{
    // Detect the used firmware
    char *firmware = "BIOS";
    if (estr.response != NULL)
        firmware = "UEFI";

    // Display the information
    Log(LT_INFO, "bootsrv", "bootloader infos:\n");
    Log(LT_INFO, "bootsrv", " - %s %s (booted on a %s %s system)\n", bir.response->name, bir.response->version, KARCHITECTURE, firmware);
    Log(LT_INFO, "bootsrv", " - kernel physical address: 0x%x\n", kar.response->physical_base);
    Log(LT_INFO, "bootsrv", " - kernel virtual address: 0x%x\n", kar.response->virtual_base);
    Log(LT_INFO, "bootsrv", " - kernel command line: \"%s\"\n", kfr.response->kernel_file->cmdline);
    
    Log(LT_INFO, "bootsrv", " - framebuffers: \n");
    for (int i = 0; i < fbr.response->framebuffer_count; i++)
    {
        Log(LT_INFO, "bootsrv", "   - fb%d: Address: %p, Width: %u, Height: %u, Bpp: %u\n", i, fbr.response->framebuffers[i]->address, fbr.response->framebuffers[i]->width, fbr.response->framebuffers[i]->height, fbr.response->framebuffers[i]->bpp);
    }
    Log(LT_INFO, "bootsrv", " - modules:\n");
    if (module_request.response) {
        for (size_t i = 0; i < module_request.response->module_count; i++)
        {
            struct limine_file *module = module_request.response->modules[i];
            Log(LT_INFO, "BootSrv", "   - module %d: path: %s, addr: %p\n", i, module->path, module->address);
        }
    }
    else
    {
        Log(LT_INFO, "bootsrv", "     <no modules provided>\n");
    }
}

struct limine_framebuffer *BootSrv_GetFramebuffer(int number)
{
    return fbr.response->framebuffers[number];
}

struct limine_memmap_response *BootSrv_GetMemoryMap()
{
    return memmap_request.response;
}

struct limine_file *BootSrv_GetModule(int pos)
{
    if (pos == -2)
        return kfr.response->kernel_file;
    else
        return module_request.response->modules[pos];
}

struct limine_smp_info *BootSrv_GetCPU(int pos)
{
    if (pos < _smp_request.response->cpu_count)
    {
        return _smp_request.response->cpus[pos];
    }
    else
    {
        return NULL;
    }
}

uint64_t *BootSrv_GetKernelPhysicalBase() { return kar.response->physical_base; }
uint64_t *BootSrv_GetKernelVirtualBase() { return kar.response->virtual_base; }