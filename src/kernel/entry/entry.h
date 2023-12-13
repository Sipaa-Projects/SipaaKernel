#ifndef __ENTRY_H
#define __ENTRY_H

#include "skgbi.h"
#include "multiboot.h"

sk_general_boot_info get_skgbi_from_multiboot(struct multiboot_info *info);
sk_general_boot_info get_skgbi_from_limine();

#endif