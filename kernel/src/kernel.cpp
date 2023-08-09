#include "global.h"

#include <arch/gdt.h>
#include <arch/idt.h>
#include <dev/serial.h>
#include <dev/pci.h>
#include <console/console.h>
#include <limine/limine.h>
#include <logging/logger.h>
#include <memory/memory.h>
#include <video/video.h>

#include <cstdint>

using namespace Sk;
using namespace Sk::Arch;
using namespace Sk::Dev;
using namespace Sk::Graphic;
using namespace Sk::Logging;
using namespace Sk::Memory;

uint64_t kernel_stack[8192];

struct limine_framebuffer_request fbr = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

extern "C" void SK_Main()
{
    Framebuffer f = Framebuffer::FromLimine(fbr.response->framebuffers[0]);
    f.UseDoubleBuffer = false;

    Global::Framebuffer = f;

    Console::Init(f);
    Serial::Init();
    Gdt::Init((uint64_t)&kernel_stack + sizeof(kernel_stack));
    Idt::Init();

    f.UseDoubleBuffer = true;
    int x = 0;
    int y = 0;
    bool ttb = true;
    bool ltr = true;

    while (1)
    {
        f.Clear(0x0);

        if (x == f.Width - 100)
            ltr = false;
        if (x == 0)
            ltr = true;

        if (y == f.Height - 100)
            ttb = false;
        if (y == 0)
            ttb = true;

        if (ltr)
            x++;
        else
            x--;

        if (ttb)
            y++;
        else
            y--;

        for (int j = y; j < y + 100; j++)
        {
            for (int i = x; i < x + 100; i++)
            {
                f.SetPixel(i, j, 0xFFFFFF);
            }
        }

        f.SwapBuffers();
    }
}