#ifndef __FB_H__
#define __FB_H__

#include <lib/stdtype.h>
#include <stddef.h>
#include <stdbool.h>
#include <boot/limine.h>

typedef struct
{
    ui32 size;
    ui32 shift;
} color_mask;

typedef struct
{
    ui64 width;
    ui64 height;
    ui64 pitch;
    ui64 bpp;
    ui32 *address;
    ui32 *frontbuf;
    ui32 *backbuf;
    bool doubleBuffered;
    color_mask red_mask;
    color_mask green_mask;
    color_mask blue_mask;
    color_mask reserved_mask;
} framebuffer;

framebuffer *fb_fromlimine(struct limine_framebuffer *lfb);
void fb_swapbuffers(framebuffer *fb);
void fb_setpixel_32bpp(framebuffer *fb, int x, int y, ui32 color);
ui32 fb_getpixel_32bpp(framebuffer *fb, int x, int y);

#endif