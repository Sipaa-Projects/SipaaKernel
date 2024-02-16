#ifndef __ENTRY_H
#define __ENTRY_H

#include "skgbi.h"

sk_general_boot_info get_skgbi_from_multiboot(struct multiboot_info* info_addr);
sk_general_boot_info get_skgbi_from_limine();

#endif