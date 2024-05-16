/// @file src/include/sipaa/framebuffer.h
/// @brief Interface to control graphic adapters - Source

#include <sipaa/framebuffer.h>  // Include the header version of this file
#include <sipaa/drv/bga.h>      // Include the BGA header
#include <sipaa/drv/vmsvgaii.h> // Include the VMware SVGA II header
#include <sipaa/bootsrv.h>      // Include the boot service, allowing to get the framebuffer from Limine
#include <sipaa/logger.h>       // include the logger

/// @brief A pointer to the framebuffer address.
FramebufferT framebuf;

void Fbuf_Initialize()
{
    struct limine_framebuffer *fb = BootSrv_GetFramebuffer(0);

    if (fb)
    {
        Log(LT_INFO, "Framebuffer", "Using the bootloader-provided framebuffer\n");

        framebuf.Address = fb->address;
        framebuf.Size = fb->height * fb->pitch * (fb->bpp / 8);

        framebuf.Mode.Width = fb->width;
        framebuf.Mode.Height = fb->height;
        framebuf.Mode.Height = fb->pitch;
        framebuf.Mode.Height = fb->bpp;

        framebuf.RedMaskShift = fb->red_mask_shift;
        framebuf.RedMaskShift = fb->red_mask_size;
        framebuf.GreenMaskShift = fb->green_mask_shift;
        framebuf.GreenMaskSize = fb->green_mask_size;
        framebuf.BlueMaskShift = fb->blue_mask_shift;
        framebuf.BlueMaskSize = fb->blue_mask_size;
    }
    
    Log(LT_INFO, "Framebuffer", "No framebuffer available!\n");
}