#include <sipaa/bootsrv.h>
#include <stddef.h>

struct multiboot_tag_framebuffer *fb;
struct multiboot_tag_mmap *mmap;

void BootSrv_Initialize(struct BootSrv_MBoot2Info* mb2i)
{
    size_t add_size = 0;

    // NOTE: We set i to 8 to skip size and reserved fields:
    for (size_t i = 8; i < mb2i->size; i += add_size) {
        struct multiboot_tag *tag = (struct multiboot_tag *)((uint8_t *)mb2i + i);

        if (tag->type == MULTIBOOT_TAG_TYPE_END) {
            break;
        }

        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                fb = (struct multiboot_tag_framebuffer *)tag;
                break;
            case MULTIBOOT_TAG_TYPE_MMAP:
                mmap = (struct multiboot_tag_mmap *)tag;
                break;
        }

        add_size = tag->size;

        // Align the size to 8 bytes.
        if ((add_size % 8) != 0)
			add_size += (8 - add_size % 8);
    }
}

struct multiboot_tag *BootSrv_GetMBoot2Tag(uint32_t tagtype)
{
    switch (tagtype)
    {
    case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
        return (struct multiboot_tag *)fb;
    case MULTIBOOT_TAG_TYPE_MMAP:
        return (struct multiboot_tag *)mmap;
    
    default:
        return NULL;
    }
}