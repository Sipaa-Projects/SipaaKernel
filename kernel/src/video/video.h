#ifndef VIDEO_H
#define VIDEO_H

#include <limine/limine.h>
#include <cstdint>

namespace Sk {
namespace Graphic {

class Framebuffer {
public:
    int Width = 0;
    int Height = 0;
    uint16_t Bpp = 0;
    uint64_t Pitch = 0;
    
    uint32_t *BackbufferAddress = 0;
    uint32_t *Address = 0;

    void* BackBuffer = (void*)0;
    void* FrontBuffer = (void*)0;

    bool UseDoubleBuffer = false;

    Framebuffer::Framebuffer(int width, int height, uint16_t bpp);
    Framebuffer::Framebuffer(int width, int height, uint16_t bpp, uint32_t *address);
    Framebuffer::Framebuffer(int width, int height, uint16_t bpp, uint64_t pitch, uint32_t *address);
    Framebuffer::Framebuffer(bool usedoublebuffer, int width, int height, uint16_t bpp, uint64_t pitch, uint32_t *address);

    static Framebuffer from_limine(limine_framebuffer *framebuf);

    void clear(uint32_t color);
    void set_pixel(int x, int y, uint32_t color);
    void swap_buffers();
};

} // namespace Graphic
} // namespace Sk

#endif // FRAMEBUFFER_H
