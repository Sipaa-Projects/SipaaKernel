#pragma once

#include <limine/limine.h>

#include <stdint.h>

namespace Sk {
namespace Video {

class Framebuffer {

public:
    int Width, Height, Bpp, Pitch;

    void SetPixel(int x, int y, uint32_t color) {}
    uint32_t GetPixel(int x, int y) { return 0; }

};

class AddressFramebuffer : public Framebuffer {

public:
    uint32_t *Address;

    static AddressFramebuffer FromLimine(limine_framebuffer *f) 
    {
        AddressFramebuffer fb = AddressFramebuffer();
        fb.Address = f->address;
        fb.Pitch = f->pitch;
        fb.Width = f->width;
        fb.Height = f->height;
        fb.Bpp = f->bpp;
        return fb;
    }
    
    void SetPixel(int x, int y, uint32_t color)
    {
        uint32_t *pixel_address = 0;

        pixel_address = (uint32_t *)((uint64_t)this->Address + y * this->Pitch + x * (this->Bpp / 8));

        *pixel_address = color;
    }

    uint32_t GetPixel(int x, int y)
    {
        uint32_t *pixel_address = 0;

        pixel_address = (uint32_t *)((uint64_t)this->Address + y * this->Pitch + x * (this->Bpp / 8));

        return *pixel_address;
    }
};

}
}