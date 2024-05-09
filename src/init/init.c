#include <stdint.h>
#include <stddef.h>
#include <sipaa/bootsrv.h>
#include <sipaa/bga.h>
#include <sipaa/i686/gdt.h>
#include <sipaa/i686/tss.h>
#include <sipaa/i686/idt.h>
#include <sipaa/pmm.h>
#include <sipaa/kdebug.h>
#include <sipaa/logger.h>

#define GDBSTUB_ARCH_X86
#define GDBSTUB_DONT_DEFINE_STDINT_TYPES

#define GDBSTUB_IMPLEMENTATION
#include <gdbstub.h>

void usr_main() {}

void SKEntry(uint32_t magic, struct BootSrv_MBoot2Info* mb2i)
{
    BootSrv_Initialize(mb2i);
    Dbg_Initialize(Com1);

    Log(LT_INFO, "Kernel", "We are here");

    Gdt_Initialize();
    Tss_Initialize();
    Idt_Initialize();

    Pmm_Initialize();

    //gdb_sys_init();

    struct multiboot_tag_framebuffer *fb = (struct multiboot_tag_framebuffer *)BootSrv_GetMBoot2Tag(MULTIBOOT_TAG_TYPE_FRAMEBUFFER);

    // Always do a NULL check after a call to BootSrv_GetMBoot2Tag, because it can return NULL incase the tag doesn't exists
    if (fb != 0)
    {
        // Fill the screen with white
        uint32_t *fbaddr = fb->common.framebuffer_addr;
        
        int size = fb->common.framebuffer_width * fb->common.framebuffer_height * fb->common.framebuffer_bpp;

        if (BochsGA_IsAvailable(VBE_DISPI_ID4))
        {
            BochsGA_SetVideoMode(1024, 768, VBE_DISPI_BPP_32, 0, 0);
            int x = 1023;
            int y = 767;
            fbaddr[1023] = 0xFFFFFF;
            //fbaddr[0] = 0xFFFFFF;
        }
        
        for(size_t x = 0; x < 1024; x++)
        {
            for(size_t y = 0; y < 768; y++)
            {
                size_t raw_position = x + y * 1024; 

                int a = 0;
                int b = x ^ y;
                int r = (y * 2) ^ (x * 2);
                int g  = (y * 4) ^ (x * 4);

                fbaddr[raw_position] = (a << 24) | (r << 16) | (g << 8) | b;
            }
        }
    }

    for(;;) ;;
}