#include "entry.h"
#include <stddef.h>
#include <device/serial/serial.h>

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};
    
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0};

struct limine_memmap_response *entry_getmemmap()
{
    return memmap_request.response;
}

struct limine_framebuffer *entry_getfb()
{
    return framebuffer_request.response->framebuffers[0];
}