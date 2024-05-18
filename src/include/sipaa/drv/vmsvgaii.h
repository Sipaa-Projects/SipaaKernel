/// @brief SipaaKernel's VMware SVGA II driver. Credit to K. Lange for the driver.

#pragma once

#include <sipaa/framebuffer.h>
#include <sipaa/pci.h>

#define SVGA_PCI_VENDOR 0x15AD
#define SVGA_PCI_DEVICE 0x0405

#define SVGA_IO_BASE (VMSVGA_Io)
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

extern uint32_t VMSVGA_Io;

bool VMSVGA_Install(FramebufferT *fb, FramebufferCapabilitiesT *cp);