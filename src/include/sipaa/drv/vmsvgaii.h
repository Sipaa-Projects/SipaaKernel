/// @brief SipaaKernel's VMware SVGA II driver. Credit to Klange for the driver.
// TODO: implement PCI.

#ifndef __VMSVGAII_H
#define __VMSVGAII_H

#define SVGA_IO_BASE (vmware_io)
#define SVGA_IO_MUL 1
#define SVGA_INDEX_PORT 0
#define SVGA_VALUE_PORT 1

#define SVGA_REG_ID 0
#define SVGA_REG_ENABLE 1
#define SVGA_REG_WIDTH 2
#define SVGA_REG_HEIGHT 3
#define SVGA_REG_BITS_PER_PIXEL 7
#define SVGA_REG_BYTES_PER_LINE 12
#define SVGA_REG_FB_START 13

/**static uint32_t vmware_io = 0;

static void vmware_scan_pci(uint32_t device, uint16_t v, uint16_t d, void * extra) {
	if ((v == 0x15ad && d == 0x0405)) {
		uintptr_t t = pci_read_field(device, PCI_BAR0, 4);
		if (t > 0) {
			*((uint8_t **)extra) = (uint8_t *)(t & 0xFFFFFFF0);
		}
	}
}

static void vmware_write(int reg, int value) {
	outportl(SVGA_IO_MUL * SVGA_INDEX_PORT + SVGA_IO_BASE, reg);
	outportl(SVGA_IO_MUL * SVGA_VALUE_PORT + SVGA_IO_BASE, value);
}

static uint32_t vmware_read(int reg) {
	outportl(SVGA_IO_MUL * SVGA_INDEX_PORT + SVGA_IO_BASE, reg);
	return inportl(SVGA_IO_MUL * SVGA_VALUE_PORT + SVGA_IO_BASE);
}

static void vmware_set_resolution(uint16_t w, uint16_t h) {
	vmware_write(SVGA_REG_ENABLE, 0);
	vmware_write(SVGA_REG_ID, 0);
	vmware_write(SVGA_REG_WIDTH, w);
	vmware_write(SVGA_REG_HEIGHT, h);
	vmware_write(SVGA_REG_BITS_PER_PIXEL, 32);
	vmware_write(SVGA_REG_ENABLE, 1);

	uint32_t bpl = vmware_read(SVGA_REG_BYTES_PER_LINE);

	lfb_resolution_x = w;
	lfb_resolution_s = bpl;
	lfb_resolution_y = h;
	lfb_resolution_b = 32;

	lfb_memsize = vmware_read(15);
}

static void graphics_install_vmware(uint16_t w, uint16_t h) {
	pci_scan(vmware_scan_pci, -1, &vmware_io);

	if (!vmware_io) {
		printf("vmware video, but no device found?\n");
		return;
	} else {
		printf("vmware io base: %p\n", (void*)(uintptr_t)vmware_io);
	}

	vmware_set_resolution(w,h);
	lfb_resolution_impl = &vmware_set_resolution;

	uintptr_t fb_addr = vmware_read(SVGA_REG_FB_START);
	printf("vmware fb address: %p\n", (void*)fb_addr);

	lfb_memsize = vmware_read(15);

	printf("vmware fb size: 0x%lx\n", lfb_memsize);

	lfb_vid_memory = mmu_map_from_physical(fb_addr);

	finalize_graphics("vmware");
}**/

#end