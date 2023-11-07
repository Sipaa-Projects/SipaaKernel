#include <entry.h>

#include "limine_hdr.h"

static struct limine_bootloader_info_request bootloader_info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0
};

static struct limine_framebuffer_request framebuf_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

static struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

struct limine_memmap_response *limine_getmmap() {
    return memmap_request.response;
}
struct limine_framebuffer_response *limine_getfbufs() {
    return framebuf_request.response;
}
struct limine_bootloader_info_response *limine_getbtinfo() {
    return bootloader_info_request.response;
}