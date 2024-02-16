#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <multiboot/multiboot2.h>
#include <flanterm/backends/fb.h>
#include <sipaa/entry.h>

void kstart(uint32_t magic, struct multiboot_info* mb2info)
{
    sk_general_boot_info skgbi = get_skgbi_from_multiboot(mb2info);

        struct flanterm_context *ctx = flanterm_fb_simple_init((uint32_t *)skgbi.fb0.address, skgbi.fb0.width, skgbi.fb0.height, skgbi.fb0.pitch);
        flanterm_write(ctx, "Hello World", 11);

    while (1)
    {

    }
}