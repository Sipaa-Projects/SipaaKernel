#include "entry.h"
#include "limine.h"
#include "skgbi.h"

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

sk_general_boot_info get_skgbi_from_multiboot(struct multiboot_info *info)
{
    sk_general_boot_info info2 = { };

    // Framebuffer
    info2.fb0.address = info->framebuffer_addr;
    info2.fb0.bpp = info->framebuffer_bpp;
    info2.fb0.width = info->framebuffer_width;
    info2.fb0.height = info->framebuffer_height;
    info2.fb0.pitch = info->framebuffer_pitch;
    info2.fb0.memory_model = 0;

    info2.fb0.red_mask_shift = info->framebuffer_red_field_position;
    info2.fb0.red_mask_size = info->framebuffer_red_mask_size;
    info2.fb0.blue_mask_shift = info->framebuffer_blue_field_position;
    info2.fb0.blue_mask_size = info->framebuffer_blue_mask_size;
    info2.fb0.green_mask_shift = info->framebuffer_green_mask_size;
    info2.fb0.green_mask_size = info->framebuffer_green_field_position;

    return info2;
}

sk_general_boot_info get_skgbi_from_limine()
{
    sk_general_boot_info info = { };

    // Framebuffer
    struct limine_framebuffer *fbptr = framebuffer_request.response->framebuffers[0];

    info.fb0.address = fbptr->address;
    info.fb0.bpp = fbptr->bpp;
    info.fb0.width = fbptr->width;
    info.fb0.height = fbptr->height;
    info.fb0.pitch = fbptr->pitch;
    info.fb0.memory_model = fbptr->memory_model;

    info.fb0.red_mask_shift = fbptr->red_mask_shift;
    info.fb0.red_mask_size = fbptr->red_mask_size;
    info.fb0.blue_mask_shift = fbptr->blue_mask_shift;
    info.fb0.blue_mask_size = fbptr->blue_mask_size;
    info.fb0.green_mask_shift = fbptr->green_mask_shift;
    info.fb0.green_mask_size = fbptr->green_mask_size;

    return info;
}