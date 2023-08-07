#include <limine/limine.h>
#include <video/video.h>

#include <cstdint>

using namespace Sk::Graphic;

struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

extern "C" void SK_Main()
{
    limine_framebuffer *lf = framebuffer_request.response->framebuffers[0];

    Framebuffer f = Framebuffer::from_limine(lf);
    
    int x = 0;
    int y = 0;
    bool ttb = true;
    bool ltr = true;

    // Hello Worldd
    while (1)
    {
        f.clear(0x0);

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
                f.set_pixel(i, j, 0xFFFFFF);
            }
        }

        f.swap_buffers();
    }
}