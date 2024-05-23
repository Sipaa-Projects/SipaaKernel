/// @file src/include/sipaa/framebuffer.h
/// @brief Interface to control graphic adapters - Source

#include <sipaa/framebuffer.h>  // Include the header version of this file
#include <sipaa/drv/bga.h>      // Include the BGA header
#include <sipaa/drv/vmsvgaii.h> // Include the VMware SVGA II header
#include <sipaa/bootsrv.h>      // Include the boot service, allowing to get the framebuffer from Limine
#include <sipaa/logger.h>       // Include the logger
#include <sipaa/pci.h>          // Include the PCI

/// @brief A pointer to the framebuffer address.
FramebufferT Framebuffer = { };
FramebufferCapabilitiesT Framebuffer_Capabilities = { };

void Fbuf_SetupBasicCapabilities()
{
    Framebuffer_Capabilities.CanBeDisabled = false;
    Framebuffer_Capabilities.Enable = (Framebuffer_EnableT)0;
    Framebuffer_Capabilities.CanSetModes = false;
    Framebuffer_Capabilities.SetMode = (Framebuffer_SetModeT)0;
    Framebuffer_Capabilities.FramebufferGraphicsAccelertion = NONE;
    Framebuffer_Capabilities.MaxMode = Framebuffer.Mode;
}

void Fbuf_Initialize()
{
    struct limine_framebuffer *fb = BootSrv_GetFramebuffer(0);

    if (fb != (void *)0)
    {
        Log(LT_INFO, "Framebuffer", "Using the bootloader-provided framebuffer\n");

        Framebuffer.Address = fb->address;
        Framebuffer.Size = fb->height * fb->pitch * (fb->bpp / 8);

        Framebuffer.Mode.Width = fb->width;
        Framebuffer.Mode.Height = fb->height;
        Framebuffer.Mode.Pitch = fb->pitch;
        Framebuffer.Mode.Bpp = fb->bpp;

        Framebuffer.RedMaskShift = fb->red_mask_shift;
        Framebuffer.RedMaskSize = fb->red_mask_size;
        Framebuffer.GreenMaskShift = fb->green_mask_shift;
        Framebuffer.GreenMaskSize = fb->green_mask_size;
        Framebuffer.BlueMaskShift = fb->blue_mask_shift;
        Framebuffer.BlueMaskSize = fb->blue_mask_size;

        Log(LT_INFO, "Framebuffer", "Initializing framebuffer capabilities structure...\n");

        Fbuf_SetupBasicCapabilities();

        Log(LT_INFO, "Framebuffer", "The framebuffer is now available\n");
        return;
    }

    Log(LT_WARNING, "Framebuffer", "No framebuffer available! Good luck with the serial console!\n");
    Framebuffer.Address = (UI64)0;
}

void Fbuf_InitializeGPU()
{
    Log(LT_INFO, "Framebuffer", "Checking for available graphics accelerations\n");

    if (Pci_Exists(0x15AD, 0x0405))
    {
        Log(LT_INFO, "Framebuffer", "Found a VMware SVGA II!\n");
        
        if (!VMSVGA_Install(&Framebuffer, &Framebuffer_Capabilities))
        {
            Log(LT_INFO, "Framebuffer", "Unfortunately, the VMware SVGA II adapter installation failed. We will stick to the bootloader-provided framebuffer.\n");
            return;
        }

        Log(LT_INFO, "Framebuffer", "Installed the VMware SVGA II!\n");
        return;
    }
    else if (BochsGA_IsAvailable(VBE_DISPI_ID2)) // SipaaKernel supports BGA starting from ID2.
    {
        Log(LT_INFO, "Framebuffer", "Found a Bochs Graphics Adapter!\n");

        Log(LT_INFO, "Framebuffer", "Installed the Bochs Graphics Adapter!\n");

        return;
    }
    else
    {
        Log(LT_WARNING, "Framebuffer", "No graphics accelerators found! GUI may be slow.\n");

        return;
    }
}

void Fbuf_SetMode(FramebufferModeT mode)
{
    if (Framebuffer_Capabilities.CanSetModes && Framebuffer_Capabilities.SetMode) {
        Framebuffer_Capabilities.SetMode(&Framebuffer, mode);
        Log(LT_INFO, "Framebuffer", "Mode has been set successfully!\n");
        return;
    }
    Log(LT_WARNING, "Framebuffer", "Cannot set mode: Operation not supported.\n");
}

FramebufferT *Fbuf_Get()
{
    return &Framebuffer;
}