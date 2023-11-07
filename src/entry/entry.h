#ifndef __LIMINE_H
#define __LIMINE_H

#include "limine/limine_hdr.h"

struct limine_memmap_response *limine_getmmap();
struct limine_framebuffer_response *limine_getfbufs();
struct limine_bootloader_info_response *limine_getbtinfo();

#endif