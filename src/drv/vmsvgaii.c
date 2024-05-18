#include <sipaa/drv/vmsvgaii.h>
#include <sipaa/logger.h>
#include <sipaa/x86_64/io.h>
#include <stdbool.h>

uint32_t VMSVGA_Io = 0;

void VMSVGA_ScanPCI(uint32_t device, uint16_t v, uint16_t d, void * extra) {
	if ((v == 0x15ad && d == 0x0405)) {
		uintptr_t t = Pci_ReadField(device, PCI_BAR0, 4);
		if (t > 0) {
			*((uint8_t **)extra) = (uint8_t *)(t & 0xFFFFFFF0);
		}
	}
}

// SVGA I/O
void VMSVGA_Write(int reg, int value) {
	outl(SVGA_IO_MUL * SVGA_INDEX_PORT + SVGA_IO_BASE, reg);
	outl(SVGA_IO_MUL * SVGA_VALUE_PORT + SVGA_IO_BASE, value);
}

uint32_t VMSVGA_Read(int reg) {
	outl(SVGA_IO_MUL * SVGA_INDEX_PORT + SVGA_IO_BASE, reg);
	return inl(SVGA_IO_MUL * SVGA_VALUE_PORT + SVGA_IO_BASE);
}

void VMSVGA_SetResolution(FramebufferT *fb, FramebufferModeT mode) {
	VMSVGA_Write(SVGA_REG_ENABLE, 0);
	VMSVGA_Write(SVGA_REG_ID, 0);
	VMSVGA_Write(SVGA_REG_WIDTH, mode.Width);
	VMSVGA_Write(SVGA_REG_HEIGHT, mode.Height);
	VMSVGA_Write(SVGA_REG_BITS_PER_PIXEL, 32);
	VMSVGA_Write(SVGA_REG_ENABLE, 1);

	uint32_t bpl = VMSVGA_Read(SVGA_REG_BYTES_PER_LINE);

	fb->Mode.Width = mode.Width;
	fb->Mode.Pitch = bpl;
	fb->Mode.Height = mode.Height;
	fb->Mode.Bpp = BPP32;

	fb->Size = VMSVGA_Read(15);
	Log(LT_WARNING, "VMwareSVGA", "Mode set\n");
}

bool VMSVGA_Install(FramebufferT *fb, FramebufferCapabilitiesT *cp)
{
    Pci_Scan(VMSVGA_ScanPCI, -1, &VMSVGA_Io);

	if (!VMSVGA_Io) {
		Log(LT_WARNING, "VMwareSVGA", "VMware video, but no device found?\n");
		return false;
	} else {
		Log(LT_INFO, "VMwareSVGA", "VMware SVGAII I/O Base: %p\n", (void*)(uintptr_t)VMSVGA_Io);
	}

    FramebufferModeT mode;
    mode.Bpp = 32;
    mode.Width = 1280;
    mode.Height = 720;
    mode.Pitch = 1280 * 4;

	VMSVGA_SetResolution(fb, mode);
    cp->CanSetModes = true;
	cp->SetMode = &VMSVGA_SetResolution;

	uint32_t *fb_addr = VMSVGA_Read(SVGA_REG_FB_START);
	Log(LT_INFO, "VMwareSVGA", "Framebuffer address: %p\n", (void*)fb_addr);

	fb->Size = VMSVGA_Read(15);
	fb->Address = /**mmu_map_from_physical(**/fb_addr/**)**/; // SipaaKernel doesn't have virtual memory management right now.

    return true;
}