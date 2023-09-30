#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include <stdint.h>
#include <stdarg.h>
#include <dev/fb/fb.h>
#include "psf.h"

typedef struct
{
        uint8_t *font;
        framebuffer *framebuffer;
        unsigned int cursor_x;
        unsigned int cursor_y;
        uint32_t fgcolor;
        uint32_t bgcolor;
} Terminal;

extern Terminal *current_terminal;
extern Terminal *main_terminal;

void init_terminal(Terminal *terminal, uint8_t *font, framebuffer *framebuffer);
void switch_terminal(Terminal *terminal);
void clear_terminal(uint32_t color);
void terminal_putpixel(unsigned int x, unsigned int y, uint32_t color);
uint32_t terminal_getpixel(unsigned int x, unsigned int y);
void print_character(unsigned char c);
void print_string(unsigned char *c);
void vprintf(unsigned char *format, va_list args);
void printf(unsigned char *format, ...);

#endif