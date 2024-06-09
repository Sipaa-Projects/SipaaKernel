/// @file src/include/sipaa/framebuffer.h
/// @brief Interface to control graphic adapters - Source

#include <sipaa/framebuffer.h>  // Include the header version of this file
#include <sipaa/dev/bga.h>      // Include the BGA header
#include <sipaa/dev/vmsvgaii.h> // Include the VMware SVGA II header
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
        Log(LT_INFO, "framebuf", "using the bootloader-provided framebuffer\n");

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

        Log(LT_INFO, "framebuf", "Initializing framebuffer capabilities structure...\n");

        Fbuf_SetupBasicCapabilities();

        Log(LT_INFO, "framebuf", "The framebuffer is now available\n");
        return;
    }

    Log(LT_WARNING, "framebuf", "no framebuffer available! good luck with the serial console!\n");
    Framebuffer.Address = (UI64)0;
}

void Fbuf_InitializeGPU()
{
    #ifdef __x86_64__
    Log(LT_INFO, "framebuf", "checking for available graphics accelerations\n");

    if (Pci_Exists(0x15AD, 0x0405))
    {
        Log(LT_INFO, "framebuf", "found a VMware SVGA II!\n");
        
        if (!VMSVGA_Install(&Framebuffer, &Framebuffer_Capabilities))
        {
            Log(LT_INFO, "framebuf", "unfortunately, the VMware SVGA II adapter installation failed. We will stick to the bootloader-provided framebuffer.\n");
            return;
        }

        Log(LT_INFO, "framebuf", "installed the VMware SVGA II!\n");
        return;
    }
    else if (BochsGA_IsAvailable(VBE_DISPI_ID2)) // SipaaKernel supports BGA starting from ID2.
    {
        Log(LT_INFO, "framebuf", "found a Bochs Graphics Adapter!\n");

        Log(LT_INFO, "framebuf", "installed the Bochs Graphics Adapter!\n");

        return;
    }
    else
    {
        Log(LT_WARNING, "framebuf", "no graphics accelerators found! GUI may be slow.\n");

        return;
    }
    #else
    Log(LT_WARNING, "framebuf", "currently implemented GPUs are only available for x86_64.\n");
    #endif
}

void Fbuf_SetMode(FramebufferModeT mode)
{
    if (Framebuffer_Capabilities.CanSetModes && Framebuffer_Capabilities.SetMode) {
        Framebuffer_Capabilities.SetMode(&Framebuffer, mode);
        Log(LT_INFO, "framebuf", "mode has been set successfully!\n");
        return;
    }
    Log(LT_WARNING, "framebuf", "cannot set mode: Operation not supported.\n");
}

FramebufferT *Fbuf_Get()
{
    return &Framebuffer;
}