/// @brief Boot service header - Allows getting some info provided by the bootloader
#ifndef __BOOTSRV_H
#define __BOOTSRV_H

#include <stdint.h>
#include <multiboot.h>

struct BootSrv_MBoot2Info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

void BootSrv_Initialize(struct BootSrv_MBoot2Info* mb2i);
struct multiboot_tag *BootSrv_GetMBoot2Tag(uint32_t tagtype);

#endif