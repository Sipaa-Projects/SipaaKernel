/*
 * @file src/include/sipaa/drv/vmsvgaII.h
 * @brief VMware SVGA II driver. Based on ToaruOS's & SerenityOS's implementation.
 * 
 * Credit to K. Lange & SerenityOS contributors.
 */

#pragma once

#include <sipaa/framebuffer.h>
#include <sipaa/pci.h>
#include <sipaa/libc/types.h>

#define SVGA_PCI_VENDOR 0x15AD
#define SVGA_PCI_DEVICE 0x0405

#define SVGA_IO_BASE (VMSVGA_Io)
#define SVGA_IO_MUL 1
#define SVGA_INDEX_PORT 0
#define SVGA_VALUE_PORT 1

enum VMwareSVGARegisters {
    ID = 0,
    ENABLE = 1,
    WIDTH = 2,
    HEIGHT = 3,
    MAX_WIDTH = 4,
    MAX_HEIGHT = 5,
    DEPTH = 6,
    BITS_PER_PIXEL = 7, /* Current bpp in the guest */
    PSEUDOCOLOR = 8,
    RED_MASK = 9,
    GREEN_MASK = 10,
    BLUE_MASK = 11,
    BYTES_PER_LINE = 12,
    FB_START = 13,
    FB_OFFSET = 14,
    VRAM_SIZE = 15,
    FB_SIZE = 16,

    CAPABILITIES = 17,
    MEM_SIZE = 19,
    CONFIG_DONE = 20,  /* Set when memory area configured */
    SYNC = 21,         /* See "FIFO Synchronization Registers" */
    BUSY = 22,         /* See "FIFO Synchronization Registers" */
    SCRATCH_SIZE = 29, /* Number of scratch registers */
    MEM_REGS = 30,     /* Number of FIFO registers */
    PITCHLOCK = 32,    /* Fixed pitch for all modes */
    IRQMASK = 33,      /* Interrupt mask */

    GMR_ID = 41,
    GMR_DESCRIPTOR = 42,
    GMR_MAX_IDS = 43,
    GMR_MAX_DESCRIPTOR_LENGTH = 44,

    TRACES = 45,         /* Enable trace-based updates even when FIFO is on */
    GMRS_MAX_PAGES = 46, /* Maximum number of 4KB pages for all GMRs */
    MEMORY_SIZE = 47,    /* Total dedicated device memory excluding FIFO */
};

typedef enum VMwareSVGARegisters VMwareSVGARegistersT;


struct VMwareSVGAFifoRegisters {
    UI32 start;
    UI32 size;
    UI32 next_command;
    UI32 stop;
    UI32 commands[];
};

typedef struct VMwareSVGAFifoRegisters VMwareSVGAFifoRegistersT;


extern uint32_t VMSVGA_Io;

bool VMSVGA_Install(FramebufferT *fb, FramebufferCapabilitiesT *cp);