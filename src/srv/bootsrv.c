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

void BootSrv_EnumerateFramebuffers()
{
    Log(LT_INFO, "BootSrv", "------ FRAMEBUFFERS ------\n");
    for (int i = 0; i < fbr.response->framebuffer_count; i++)
    {
        Log(LT_INFO, "BootSrv", "Framebuffer %d: Address: %p, Width: %u, Height: %u, Bpp: %u\n", i, fbr.response->framebuffers[i]->address, fbr.response->framebuffers[i]->width, fbr.response->framebuffers[i]->height, fbr.response->framebuffers[i]->bpp);
    }
}

void BootSrv_EnumerateProtocolInfos()
{
    // Detect the used firmware
    char *firmware = "BIOS";
    if (estr.response != NULL)
        firmware = "UEFI";

    // Display the information
    Log(LT_INFO, "BootSrv", "-- PROOCOL INFO START ------\n");
    Log(LT_INFO, "BootSrv", "%s %s (booted on a %s %s system)\n", bir.response->name, bir.response->version, KARCHITECTURE, firmware);
    Log(LT_INFO, "BootSrv", "Kernel physical address: \"0x%x\"\n", kar.response->physical_base);
    Log(LT_INFO, "BootSrv", "Kernel virtual address: \"0x%x\"\n", kar.response->virtual_base);
    Log(LT_INFO, "BootSrv", "Kernel command line: \"%s\"\n", kfr.response->kernel_file->cmdline);
    BootSrv_EnumerateFramebuffers();
    
    Log(LT_INFO, "BootSrv", "----- MODULES -----\n");
    for (size_t i = 0; i < module_request.response->module_count; i++)
    {
        struct limine_file *module = module_request.response->modules[i];
        Log(LT_INFO, "BootSrv", "Module %d: Path: %s, Address: %p\n", i, module->path, module->address);
    }

    Log(LT_INFO, "BootSrv", "-------- PROOCOL INFO END --\n");

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
    return module_request.response->modules[pos];
}
