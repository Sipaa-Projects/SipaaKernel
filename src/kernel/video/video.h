#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <limine/limine.h>
#include "font.h"

typedef struct
{
    int x;
    int y;
} pos;

extern struct limine_framebuffer *current_framebuffer;
extern void *frontbuf;
extern void *backbuf;
extern bool usedoublebuffering;

void init_video(struct limine_framebuffer *framebuffer);
void draw_pixel(int x, int y, uint32_t color);
void draw_rectangle(int x, int y, int width, int height, uint32_t color);
uint32_t get_pixel(int x, int y);
void clear_screen(uint32_t color);
void draw_char(char c, int x, int y, uint32_t color);
pos draw_string(char *string, int x, int y, uint32_t col);
void swap_buffers(void);


#endif