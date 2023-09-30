#ifndef __FB_H__
#define __FB_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <boot/limine.h>

typedef struct
{
    uint32_t size;
    uint32_t shift;
} color_mask;

typedef struct
{
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
    uint64_t bpp;
    uint32_t *address;
    uint32_t *frontbuf;
    uint32_t *backbuf;
    bool doubleBuffered;
    color_mask red_mask;
    color_mask green_mask;
    color_mask blue_mask;
    color_mask reserved_mask;
} framebuffer;

framebuffer *fb_fromlimine(struct limine_framebuffer *lfb);
void fb_swapbuffers(framebuffer *fb);
void fb_setpixel_32bpp(framebuffer *fb, int x, int y, uint32_t color);
uint32_t fb_getpixel_32bpp(framebuffer *fb, int x, int y);

#endif