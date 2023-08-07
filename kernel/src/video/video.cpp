#include "video.h"

#include <memory/memory.h>

using namespace Sk::Memory;

namespace Sk {
namespace Graphic {

Framebuffer::Framebuffer(int width, int height, uint16_t bpp)
    : Width(width), Height(height), Bpp(bpp) {

}

Framebuffer::Framebuffer(int width, int height, uint16_t bpp, uint32_t *address)
    : Width(width), Height(height), Bpp(bpp), Address(address), FrontBuffer((void*)address) {

}

Framebuffer::Framebuffer(int width, int height, uint16_t bpp, uint64_t pitch, uint32_t *address)
    : Width(width), Height(height), Bpp(bpp), Pitch(pitch), Address(address), FrontBuffer((void*)address) {

}

Framebuffer::Framebuffer(bool usedoublebuffer, int width, int height, uint16_t bpp, uint64_t pitch, uint32_t *address)
    : UseDoubleBuffer(usedoublebuffer), Width(width), Height(height), Bpp(bpp), Pitch(pitch), Address(address), FrontBuffer((void*)address) {

    if (usedoublebuffer)
    {
        BackBuffer = (void *)((uint64_t)address + pitch * height);
        BackbufferAddress = (uint32_t *)((uint64_t)address + pitch * height);
    }
}

Framebuffer Framebuffer::from_limine(limine_framebuffer *framebuf) {
    // Implementation of from_limine method
    // Create and return a new Framebuffer object
    return Framebuffer(true, (int)framebuf->width, (int)framebuf->height, framebuf->bpp, framebuf->pitch, (uint32_t*)framebuf->address);
}

void Framebuffer::clear(uint32_t color)
{
    for (int j = 0; j < this->Height; j++)
    {
        for (int i = 0; i < this->Width; i++)
        {
            this->set_pixel(i, j, color);
        }
    }
}

void Framebuffer::set_pixel(int x, int y, uint32_t color) {
    uint32_t *pixel_address = 0;

    if (this->UseDoubleBuffer)
        pixel_address = (uint32_t *)((uint64_t)this->BackBuffer + y * this->Pitch + x * (this->Bpp / 8));
    else
        pixel_address = (uint32_t *)((uint64_t)this->FrontBuffer + y * this->Pitch + x * (this->Bpp / 8));

    *pixel_address = color;
}

void Framebuffer::swap_buffers() {
    if (!this->UseDoubleBuffer)
        return;

    void *temp = this->FrontBuffer;
    this->FrontBuffer = this->BackBuffer;
    this->BackBuffer = temp;

    BasicMemoryManagement::MemoryCopy(this->Address, this->FrontBuffer, this->Pitch * this->Height);
}

} // namespace Graphic
} // namespace Sk
