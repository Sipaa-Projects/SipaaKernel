#include <dev/fb/fb.h>

framebuffer *fb_fromlimine(struct limine_framebuffer *lfb)
{
    framebuffer *fb = malloc(sizeof(framebuffer));

    fb->width = lfb->width;
    fb->height = lfb->height;
    fb->pitch = lfb->pitch;
    fb->address = lfb->address;
    fb->frontbuf = lfb->address;
    fb->backbuf = (void *)((ui64)lfb->address + lfb->pitch * lfb->height);
    fb->doubleBuffered = false;
    fb->bpp = 32;
    fb->red_mask.shift = lfb->red_mask_shift;
    fb->red_mask.size = lfb->red_mask_size;
    fb->green_mask.shift = lfb->green_mask_shift;
    fb->green_mask.size = lfb->green_mask_size;
    fb->blue_mask.shift = lfb->blue_mask_shift;
    fb->blue_mask.size = lfb->blue_mask_size;
    fb->reserved_mask.shift = 0;
    fb->reserved_mask.size = 0;

    return fb;
}

void fb_swapbuffers(framebuffer *fb)
{
    if (!fb->doubleBuffered)
        return;
        
    void *temp = fb->frontbuf;
    fb->frontbuf = fb->backbuf;
    fb->backbuf = temp;

    memcpy(fb->address, fb->frontbuf, fb->pitch * fb->height);
}

void fb_setpixel_32bpp(framebuffer *fb, int x, int y, ui32 color)
{
    if (fb->doubleBuffered)
    {
        ui32 *pixel_address = (ui32 *)((ui64)fb->backbuf + y * fb->pitch + x * (fb->bpp / 8));

        *pixel_address = color;
    }
    else
    {
        ui32 *pixel_address = (ui32 *)((ui64)fb->frontbuf + y * fb->pitch + x * (fb->bpp / 8));

        *pixel_address = color;
    }
}

ui32 fb_getpixel_32bpp(framebuffer *fb, int x, int y)
{
    if (fb->doubleBuffered)
    {
        return (ui32 *)((ui64)fb->backbuf + y * fb->pitch + x * (fb->bpp / 8));
    }
    else
    {
        return (ui32 *)((ui64)fb->frontbuf + y * fb->pitch + x * (fb->bpp / 8));
    }
}