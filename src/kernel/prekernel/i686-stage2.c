// SKB_i686_ONLY
#include <sipaa/entry.h>

extern void kstart(sk_general_boot_info skgbi);

void prekern_stage2(uint32_t magic, struct multiboot_info* mb2info)
{
    sk_general_boot_info skgbi = get_skgbi_from_multiboot(mb2info);

    kstart(skgbi);

    while (1)
    {

    }
}