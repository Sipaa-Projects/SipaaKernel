#include "entry.h"
#include "limine.h"
#include "skgbi.h"
#include <stddef.h>

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};
    
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0};

sk_general_boot_info get_skgbi_from_multiboot(struct multiboot_info* info_addr)
{
    sk_general_boot_info info2 = { };
    struct multiboot_tag_framebuffer *fb = NULL;
    struct multiboot_tag_mmap *mmap = NULL;

    // Get tags, so we can fill the SKGBI infos
    size_t add_size = 0;

    // NOTE: We set i to 8 to skip size and reserved fields:
    for (size_t i = 8; i < info_addr->size; i += add_size) {
        struct multiboot_tag *tag = (struct multiboot_tag *)((uint8_t *)info_addr + i);

        if (tag->type == MULTIBOOT_TAG_TYPE_END) {
            break;
        }

        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
                fb = (struct multiboot_tag_framebuffer *)tag;
                break;
            }
            case MULTIBOOT_TAG_TYPE_MMAP: {
                fb = (struct multiboot_tag_mmap *)tag;
                break;
            }
        }

        add_size = tag->size;

        // Align the size to 8 bytes.
        if ((add_size % 8) != 0)
			add_size += (8 - add_size % 8);
    }

    // Framebuffer
    info2.fb0.address = fb->common.framebuffer_addr;
    info2.fb0.bpp = fb->common.framebuffer_bpp;
    info2.fb0.width = fb->common.framebuffer_width;
    info2.fb0.height = fb->common.framebuffer_height;
    info2.fb0.pitch = fb->common.framebuffer_pitch;
    info2.fb0.memory_model = 0;

    info2.fb0.red_mask_shift = fb->framebuffer_red_field_position;
    info2.fb0.red_mask_size = fb->framebuffer_red_mask_size;
    info2.fb0.blue_mask_shift = fb->framebuffer_blue_field_position;
    info2.fb0.blue_mask_size = fb->framebuffer_blue_mask_size;
    info2.fb0.green_mask_shift = fb->framebuffer_green_mask_size;
    info2.fb0.green_mask_size = fb->framebuffer_green_field_position;

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

    // Memory map
    info.memory_map.length = 0;
    for (uint64_t entry = 0; entry < memmap_request.response->entry_count; entry++)
    {
        struct limine_memmap_entry *mentry = memmap_request.response->entries[entry];

        info.memory_map.entries[entry].base = mentry->base;
        info.memory_map.entries[entry].length = mentry->length;

        // SKGBI uses Limine memmap types.
        info.memory_map.entries[entry].type = mentry->type;

        info.memory_map.length++;
    }

    return info;
}