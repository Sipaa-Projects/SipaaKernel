#pragma once

#include <limine/limine.h>
#include <stdint.h>

namespace Sk {
namespace Graphic {

/// @brief Class used for basic rendering to a framebuffer.
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

    static Framebuffer FromLimine(limine_framebuffer *framebuf);

    void Clear(uint32_t color);
    uint32_t GetPixel(int x, int y);
    void SetPixel(int x, int y, uint32_t color);
    void SwapBuffers();
};

} // namespace Graphic
} // namespace Sk