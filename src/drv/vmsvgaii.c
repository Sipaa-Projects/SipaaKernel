/// @brief VMware SVGA II driver (WIP)

#include <sipaa/drv/vmsvgaii.h>
#include <sipaa/logger.h>
#include <sipaa/x86_64/io.h>
#include <stdbool.h>
#include <stdint.h>

uint32_t VMSVGA_Io = 0;
int vmware_svga_version_2_id = (0x900000UL << 8 | (2));

void VMSVGA_ScanPCI(uint32_t device, uint16_t v, uint16_t d, void * extra) {
	if ((v == 0x15ad && d == 0x0405)) {
		uintptr_t t = Pci_ReadField(device, PCI_BAR0, 4);
		if (t > 0) {
			*((uint8_t **)extra) = (uint8_t *)(t & 0xFFFFFFF0);
		};
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
	VMSVGA_Write(ENABLE, 0);
	//VMSVGA_Write(ID, 0);
	VMSVGA_Write(WIDTH, mode.Width);
	VMSVGA_Write(HEIGHT, mode.Height);
	VMSVGA_Write(BITS_PER_PIXEL, 32);
	VMSVGA_Write(ENABLE, 1);

	int newfbsize = VMSVGA_Read(FB_SIZE);

	uint32_t bpl = VMSVGA_Read(BYTES_PER_LINE);

	fb->Mode.Width = mode.Width;
	fb->Mode.Pitch = bpl;
	fb->Mode.Height = mode.Height;
	fb->Mode.Bpp = BPP32;

	fb->Size = newfbsize;
}

bool VMSVGA_NegotiateDeviceVersion()
{
    VMSVGA_Write(ID, vmware_svga_version_2_id);
    int accepted_version = VMSVGA_Read(ID);
    Log(LT_INFO, "VMwareSVGA", "Accepted version %d\n", accepted_version);
    if (accepted_version == vmware_svga_version_2_id)
        return true;
    return false;
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

	if (!VMSVGA_NegotiateDeviceVersion())
	{
		Log(LT_ERROR, "VMwareSVGA", "The current virtualizer does not support VMware SVGA II.\n");
		return false;
	}

    FramebufferModeT mode;
    mode.Bpp = 32;
    mode.Width = 1280;
    mode.Height = 720;
    mode.Pitch = 1280 * 4;

	VMSVGA_SetResolution(fb, mode);
    cp->CanSetModes = true;
	cp->SetMode = &VMSVGA_SetResolution;

	uint32_t *fb_addr = VMSVGA_Read(FB_START);
	Log(LT_INFO, "VMwareSVGA", "Framebuffer address: %p\n", (void*)fb_addr);

	fb->Size = VMSVGA_Read(FB_SIZE);
	fb->Address = /**mmu_map_from_physical(**/fb_addr/**)**/; // SipaaKernel doesn't have virtual memory management right now.

	// Capabilities
	uint32_t svga_capabilities = VMSVGA_Read(17);

	Log(LT_INFO, "VMwareSVGA", "Capabilities: \n");
	if (svga_capabilities & (1 << 1))
	Log(LT_INFO, "VMwareSVGA", "* Rect copy\n");
    if (svga_capabilities & (1 << 5))
	Log(LT_INFO, "VMwareSVGA", "* HW cursor\n");
    if (svga_capabilities & (1 << 6))
	Log(LT_INFO, "VMwareSVGA", "* HW cursor bypass\n");
    if (svga_capabilities & (1 << 7))
	Log(LT_INFO, "VMwareSVGA", "* HW cursor bypass é\n");
    if (svga_capabilities & (1 << 8))
	Log(LT_INFO, "VMwareSVGA", "* 8bit emu\n");
    if (svga_capabilities & (1 << 9))
	Log(LT_INFO, "VMwareSVGA", "* Alpha cursor\n");
    if (svga_capabilities & (1 << 14))
	Log(LT_INFO, "VMwareSVGA", "* 3D accel\n");
    if (svga_capabilities & (1 << 15))
	Log(LT_INFO, "VMwareSVGA", "* FIFOEx\n");
    if (svga_capabilities & (1 << 16))
	Log(LT_INFO, "VMwareSVGA", "* LegacyMultiMonitor\n");
    if (svga_capabilities & (1 << 17))
	Log(LT_INFO, "VMwareSVGA", "* PitchLock\n");
    if (svga_capabilities & (1 << 18))
	Log(LT_INFO, "VMwareSVGA", "* IRQMask\n");
    if (svga_capabilities & (1 << 19))
	Log(LT_INFO, "VMwareSVGA", "* DispTopology\n");
    if (svga_capabilities & (1 << 20))
	Log(LT_INFO, "VMwareSVGA", "* GMR\n");
    if (svga_capabilities & (1 << 21))
		Log(LT_INFO, "VMwareSVGA", "* Traces\n");
    if (svga_capabilities & (1 << 22))
		Log(LT_INFO, "VMwareSVGA", "* GMR2\n");
    if (svga_capabilities & (1 << 23))
		Log(LT_INFO, "VMwareSVGA", "* ScreenObj2\n");
    return true;
}