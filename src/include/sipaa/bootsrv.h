/// @brief Boot service header - Allows getting some info provided by the bootloader
#ifndef __BOOTSRV_H
#define __BOOTSRV_H

#include <stdint.h>
#include <limine.h>

struct BootSrv_MBoot2Info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

/// @brief Enumerate all infos provided by Limine.
void BootSrv_EnumerateProtocolInfos();

/// @brief Get the ACPI RSDP address.
struct limine_rsdp_response *BootSrv_GetAcpiRSDP();

/// @brief Get a framebuffer from Limine.
struct limine_framebuffer *BootSrv_GetFramebuffer(int number);

/// @brief Get a module provided by Limine
/// @remark If the pos argument is -2, the kernel file will be returned.
struct limine_file *BootSrv_GetModule(int pos);

/// @brief Get the memory map - Required for the PMM
struct limine_memmap_response *BootSrv_GetMemoryMap();

/// @brief Get the HHDM offset for converting physical addresses to virtual address and vice-versa.
/// @return The HHDM offset
uint64_t BootSrv_GetHHDMOffset();

/// @brief Get a pointer to the physical base of the kernel.
/// @return A pointer to the physical base of the kernel
uint64_t *BootSrv_GetKernelPhysicalBase();

/// @brief Get a pointer to the virtual base of the kernel.
/// @return A pointer to the virtual base of the kernel
uint64_t *BootSrv_GetKernelVirtualBase();

/// @brief Get a CPU
/// @return SMP information of the CPU. NULL if the cpu pos is too high.
struct limine_smp_info *BootSrv_GetCPU(int pos);

#endif