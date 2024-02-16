#ifndef __ENTRY_H
#define __ENTRY_H

#include "limine.h"

struct limine_memmap_response *entry_getmemmap();
struct limine_framebuffer *entry_getfb();

#endif