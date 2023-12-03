#include "video.h"
#include <device/serial/serial.h>
#include <stdbool.h>
#include <lib/libconsole/libconsole.h>
#include <lib/liblogging/liblogging.h>

// Graphics
struct limine_framebuffer *current_framebuffer;

bool usedoublebuffering = false;
void *frontbuf;
void *backbuf;

void init_video(struct limine_framebuffer *framebuffer)
{
    current_framebuffer = framebuffer;
    frontbuf = framebuffer->address;
    backbuf = (void *)((uint64_t)framebuffer->address + framebuffer->pitch * framebuffer->height);
}

void draw_pixel(int x, int y, uint32_t color)
{
    if (usedoublebuffering)
    {
        uint32_t *pixel_address = (uint32_t *)((uint64_t)backbuf + y * current_framebuffer->pitch + x * (current_framebuffer->bpp / 8));

        *pixel_address = color;
    }
    else
    {
        uint32_t *pixel_address = (uint32_t *)((uint64_t)frontbuf + y * current_framebuffer->pitch + x * (current_framebuffer->bpp / 8));

        *pixel_address = color;
    }
}

void draw_rectangle(int x, int y, int width, int height, uint32_t color)
{
    for (int i = x; i < x + width; i++)
    {
        for (int j = y; j < y + height; j++)
        {
            draw_pixel(i, j, color);
        }
    }
}

uint32_t get_pixel(int x, int y)
{
    if (usedoublebuffering)
    {
        uint32_t *pixel_address = (uint32_t *)((uint64_t)backbuf + y * current_framebuffer->pitch + x * (current_framebuffer->bpp / 8));

        return *pixel_address;
    }
    else
    {
        uint32_t *pixel_address = (uint32_t *)((uint64_t)frontbuf + y * current_framebuffer->pitch + x * (current_framebuffer->bpp / 8));

        return *pixel_address;
    }
}

void clear_screen(uint32_t color)
{
    term_x = 0;
    term_y = 0;

    for (int y = 0; y < current_framebuffer->height; y++)
    {
        for (int x = 0; x < current_framebuffer->width; x++)
        {
            draw_pixel(x, y, color);
        }
    }
}

void draw_char(char c, int x, int y, uint32_t color)
{
    uint8_t *offset = font + sizeof(font_header) + 16 * c;

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (offset[i] & (1 << j))
            {
                draw_pixel(x + 8 - j, y + i, color);
            }
        }
    }
}

pos draw_string(char *string, int x, int y, uint32_t col)
{
    int cursorx = x;
    int cursory = y;

    for (int i = 0; string[i] != '\0'; i++)
    {
        if (string[i] == '\n')
        {
            cursorx = x;
            cursory += 16;
        }
        else
        {
            draw_char(string[i], cursorx, cursory, col);
            cursorx += 8;
        }
    }

    pos p;
    p.x = cursorx;
    p.y = cursory;

    return p;
}

void swap_buffers(void)
{
    if (!usedoublebuffering)
    {
        log(LOG_ERROR, "swap_buffers() isn't available in single buffering.\n");
        return;
    }
    void *temp = frontbuf;
    frontbuf = backbuf;
    backbuf = temp;

    memcpy(current_framebuffer->address, frontbuf, current_framebuffer->pitch * current_framebuffer->height);

    /**for (uint64_t i = 0; i < current_framebuffer->pitch * current_framebuffer->height; i++)
    {
        ((uint8_t *)current_framebuffer->address)[i] = ((uint8_t *)frontbuf)[i];
    }**/
}
