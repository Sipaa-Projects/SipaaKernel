/* BSD Zero Clause License */

/* Copyright (C) 2022-2023 mintsuki and contributors.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* This header has been modified to remove features marked as deprecated & to be compatible with SipaaKernel */

#ifndef _LIMINE_H
#define _LIMINE_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <lib/stdtype.h>

/* Misc */

#ifdef LIMINE_NO_POINTERS
#  define LIMINE_PTR(TYPE) ui64
#else
#  define LIMINE_PTR(TYPE) TYPE
#endif

#ifdef __GNUC__
#  define LIMINE_DEPRECATED __attribute__((__deprecated__))
#  define LIMINE_DEPRECATED_IGNORE_START \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#  define LIMINE_DEPRECATED_IGNORE_END \
    _Pragma("GCC diagnostic pop")
#else
#  define LIMINE_DEPRECATED
#  define LIMINE_DEPRECATED_IGNORE_START
#  define LIMINE_DEPRECATED_IGNORE_END
#endif

#define LIMINE_COMMON_MAGIC 0xc7b1dd30df4c8b88, 0x0a82e883a194f07b

struct limine_uuid {
    ui32 a;
    ui16 b;
    ui16 c;
    ui8 d[8];
};

#define LIMINE_MEDIA_TYPE_GENERIC 0
#define LIMINE_MEDIA_TYPE_OPTICAL 1
#define LIMINE_MEDIA_TYPE_TFTP 2

struct limine_file {
    ui64 revision;
    LIMINE_PTR(void *) address;
    ui64 size;
    LIMINE_PTR(char *) path;
    LIMINE_PTR(char *) cmdline;
    ui32 media_type;
    ui32 unused;
    ui32 tftp_ip;
    ui32 tftp_port;
    ui32 partition_index;
    ui32 mbr_disk_id;
    struct limine_uuid gpt_disk_uuid;
    struct limine_uuid gpt_part_uuid;
    struct limine_uuid part_uuid;
};

/* Boot info */

#define LIMINE_BOOTLOADER_INFO_REQUEST { LIMINE_COMMON_MAGIC, 0xf55038d8e2a1202f, 0x279426fcf5f59740 }

struct limine_bootloader_info_response {
    ui64 revision;
    LIMINE_PTR(char *) name;
    LIMINE_PTR(char *) version;
};

struct limine_bootloader_info_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_bootloader_info_response *) response;
};

/* Stack size */

#define LIMINE_STACK_SIZE_REQUEST { LIMINE_COMMON_MAGIC, 0x224ef0460a8e8926, 0xe1cb0fc25f46ea3d }

struct limine_stack_size_response {
    ui64 revision;
};

struct limine_stack_size_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_stack_size_response *) response;
    ui64 stack_size;
};

/* HHDM */

#define LIMINE_HHDM_REQUEST { LIMINE_COMMON_MAGIC, 0x48dcf1cb8ad2b852, 0x63984e959a98244b }

struct limine_hhdm_response {
    ui64 revision;
    ui64 offset;
};

struct limine_hhdm_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_hhdm_response *) response;
};

/* Framebuffer */

#define LIMINE_FRAMEBUFFER_REQUEST { LIMINE_COMMON_MAGIC, 0x9d5827dcd881dd75, 0xa3148604f6fab11b }

#define LIMINE_FRAMEBUFFER_RGB 1

struct limine_video_mode {
    ui64 pitch;
    ui64 width;
    ui64 height;
    ui16 bpp;
    ui8 memory_model;
    ui8 red_mask_size;
    ui8 red_mask_shift;
    ui8 green_mask_size;
    ui8 green_mask_shift;
    ui8 blue_mask_size;
    ui8 blue_mask_shift;
};

struct limine_framebuffer {
    LIMINE_PTR(void *) address;
    ui64 width;
    ui64 height;
    ui64 pitch;
    ui16 bpp;
    ui8 memory_model;
    ui8 red_mask_size;
    ui8 red_mask_shift;
    ui8 green_mask_size;
    ui8 green_mask_shift;
    ui8 blue_mask_size;
    ui8 blue_mask_shift;
    ui8 unused[7];
    ui64 edid_size;
    LIMINE_PTR(void *) edid;
    /* Response revision 1 */
    ui64 mode_count;
    LIMINE_PTR(struct limine_video_mode **) modes;
};

struct limine_framebuffer_response {
    ui64 revision;
    ui64 framebuffer_count;
    LIMINE_PTR(struct limine_framebuffer **) framebuffers;
};

struct limine_framebuffer_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_framebuffer_response *) response;
};

/* Paging mode */

#define LIMINE_PAGING_MODE_REQUEST { LIMINE_COMMON_MAGIC, 0x95c1a0edab0944cb, 0xa4e5cb3842f7488a }

#if defined (__x86_64__) || defined (__i386__)
#define LIMINE_PAGING_MODE_X86_64_4LVL 0
#define LIMINE_PAGING_MODE_X86_64_5LVL 1
#define LIMINE_PAGING_MODE_MAX LIMINE_PAGING_MODE_X86_64_5LVL
#define LIMINE_PAGING_MODE_DEFAULT LIMINE_PAGING_MODE_X86_64_4LVL
#elif defined (__aarch64__)
#define LIMINE_PAGING_MODE_AARCH64_4LVL 0
#define LIMINE_PAGING_MODE_AARCH64_5LVL 1
#define LIMINE_PAGING_MODE_MAX LIMINE_PAGING_MODE_AARCH64_5LVL
#define LIMINE_PAGING_MODE_DEFAULT LIMINE_PAGING_MODE_AARCH64_4LVL
#elif defined (__riscv) && (__riscv_xlen == 64)
#define LIMINE_PAGING_MODE_RISCV_SV39 0
#define LIMINE_PAGING_MODE_RISCV_SV48 1
#define LIMINE_PAGING_MODE_RISCV_SV57 2
#define LIMINE_PAGING_MODE_MAX LIMINE_PAGING_MODE_RISCV_SV57
#define LIMINE_PAGING_MODE_DEFAULT LIMINE_PAGING_MODE_RISCV_SV48
#else
#error Unknown architecture
#endif

struct limine_paging_mode_response {
    ui64 revision;
    ui64 mode;
    ui64 flags;
};

struct limine_paging_mode_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_paging_mode_response *) response;
    ui64 mode;
    ui64 flags;
};

/* SMP */

#define LIMINE_SMP_REQUEST { LIMINE_COMMON_MAGIC, 0x95a67b819a1b857e, 0xa0b61b723b6a73e0 }

struct limine_smp_info;

typedef void (*limine_goto_address)(struct limine_smp_info *);

#if defined (__x86_64__) || defined (__i386__)

#define LIMINE_SMP_X2APIC (1 << 0)

struct limine_smp_info {
    ui32 processor_id;
    ui32 lapic_id;
    ui64 reserved;
    LIMINE_PTR(limine_goto_address) goto_address;
    ui64 extra_argument;
};

struct limine_smp_response {
    ui64 revision;
    ui32 flags;
    ui32 bsp_lapic_id;
    ui64 cpu_count;
    LIMINE_PTR(struct limine_smp_info **) cpus;
};

#elif defined (__aarch64__)

struct limine_smp_info {
    ui32 processor_id;
    ui32 gic_iface_no;
    ui64 mpidr;
    ui64 reserved;
    LIMINE_PTR(limine_goto_address) goto_address;
    ui64 extra_argument;
};

struct limine_smp_response {
    ui64 revision;
    ui64 flags;
    ui64 bsp_mpidr;
    ui64 cpu_count;
    LIMINE_PTR(struct limine_smp_info **) cpus;
};

#elif defined (__riscv) && (__riscv_xlen == 64)

struct limine_smp_info {
    ui64 processor_id;
    ui64 hartid;
    ui64 reserved;
    LIMINE_PTR(limine_goto_address) goto_address;
    ui64 extra_argument;
};

struct limine_smp_response {
    ui64 revision;
    ui64 flags;
    ui64 bsp_hartid;
    ui64 cpu_count;
    LIMINE_PTR(struct limine_smp_info **) cpus;
};

#else
#error Unknown architecture
#endif

struct limine_smp_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_smp_response *) response;
    ui64 flags;
};

/* Memory map */

#define LIMINE_MEMMAP_REQUEST { LIMINE_COMMON_MAGIC, 0x67cf3d9d378a806f, 0xe304acdfc50c3c62 }

#define LIMINE_MEMMAP_USABLE                 0
#define LIMINE_MEMMAP_RESERVED               1
#define LIMINE_MEMMAP_ACPI_RECLAIMABLE       2
#define LIMINE_MEMMAP_ACPI_NVS               3
#define LIMINE_MEMMAP_BAD_MEMORY             4
#define LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE 5
#define LIMINE_MEMMAP_KERNEL_AND_MODULES     6
#define LIMINE_MEMMAP_FRAMEBUFFER            7

struct limine_memmap_entry {
    ui64 base;
    ui64 length;
    ui64 type;
};

struct limine_memmap_response {
    ui64 revision;
    ui64 entry_count;
    LIMINE_PTR(struct limine_memmap_entry **) entries;
};

struct limine_memmap_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_memmap_response *) response;
};

/* Entry point */

#define LIMINE_ENTRY_POINT_REQUEST { LIMINE_COMMON_MAGIC, 0x13d86c035a1cd3e1, 0x2b0caa89d8f3026a }

typedef void (*limine_entry_point)(void);

struct limine_entry_point_response {
    ui64 revision;
};

struct limine_entry_point_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_entry_point_response *) response;
    LIMINE_PTR(limine_entry_point) entry;
};

/* Kernel File */

#define LIMINE_KERNEL_FILE_REQUEST { LIMINE_COMMON_MAGIC, 0xad97e90e83f1ed67, 0x31eb5d1c5ff23b69 }

struct limine_kernel_file_response {
    ui64 revision;
    LIMINE_PTR(struct limine_file *) kernel_file;
};

struct limine_kernel_file_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_kernel_file_response *) response;
};

/* Module */

#define LIMINE_MODULE_REQUEST { LIMINE_COMMON_MAGIC, 0x3e7e279702be32af, 0xca1c4f3bd1280cee }

#define LIMINE_INTERNAL_MODULE_REQUIRED (1 << 0)

struct limine_internal_module {
    LIMINE_PTR(const char *) path;
    LIMINE_PTR(const char *) cmdline;
    ui64 flags;
};

struct limine_module_response {
    ui64 revision;
    ui64 module_count;
    LIMINE_PTR(struct limine_file **) modules;
};

struct limine_module_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_module_response *) response;

    /* Request revision 1 */
    ui64 internal_module_count;
    LIMINE_PTR(struct limine_internal_module **) internal_modules;
};

/* RSDP */

#define LIMINE_RSDP_REQUEST { LIMINE_COMMON_MAGIC, 0xc5e77b6b397e7b43, 0x27637845accdcf3c }

struct limine_rsdp_response {
    ui64 revision;
    LIMINE_PTR(void *) address;
};

struct limine_rsdp_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_rsdp_response *) response;
};

/* SMBIOS */

#define LIMINE_SMBIOS_REQUEST { LIMINE_COMMON_MAGIC, 0x9e9046f11e095391, 0xaa4a520fefbde5ee }

struct limine_smbios_response {
    ui64 revision;
    LIMINE_PTR(void *) entry_32;
    LIMINE_PTR(void *) entry_64;
};

struct limine_smbios_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_smbios_response *) response;
};

/* EFI system table */

#define LIMINE_EFI_SYSTEM_TABLE_REQUEST { LIMINE_COMMON_MAGIC, 0x5ceba5163eaaf6d6, 0x0a6981610cf65fcc }

struct limine_efi_system_table_response {
    ui64 revision;
    LIMINE_PTR(void *) address;
};

struct limine_efi_system_table_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_efi_system_table_response *) response;
};

/* Boot time */

#define LIMINE_BOOT_TIME_REQUEST { LIMINE_COMMON_MAGIC, 0x502746e184c088aa, 0xfbc5ec83e6327893 }

struct limine_boot_time_response {
    ui64 revision;
    i64 boot_time;
};

struct limine_boot_time_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_boot_time_response *) response;
};

/* Kernel address */

#define LIMINE_KERNEL_ADDRESS_REQUEST { LIMINE_COMMON_MAGIC, 0x71ba76863cc55f63, 0xb2644a48c516a487 }

struct limine_kernel_address_response {
    ui64 revision;
    ui64 physical_base;
    ui64 virtual_base;
};

struct limine_kernel_address_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_kernel_address_response *) response;
};

/* Device Tree Blob */

#define LIMINE_DTB_REQUEST { LIMINE_COMMON_MAGIC, 0xb40ddb48fb54bac7, 0x545081493f81ffb7 }

struct limine_dtb_response {
    ui64 revision;
    LIMINE_PTR(void *) dtb_ptr;
};

struct limine_dtb_request {
    ui64 id[4];
    ui64 revision;
    LIMINE_PTR(struct limine_dtb_response *) response;
};

#ifdef __cplusplus
}
#endif

#endif