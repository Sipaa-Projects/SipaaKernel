#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include <lib/stdtype.h>
#include <stdarg.h>
#include <dev/fb/fb.h>

typedef struct
{
        ui8 *font;
        framebuffer *framebuffer;
        unsigned int cursor_x;
        unsigned int cursor_y;
        ui32 fgcolor;
        ui32 bgcolor;
} Terminal;

extern Terminal *current_terminal;
extern Terminal *main_terminal;

void init_terminal(Terminal *terminal, ui8 *font, framebuffer *framebuffer);
void switch_terminal(Terminal *terminal);
void clear_terminal(ui32 color);
void terminal_putpixel(unsigned int x, unsigned int y, ui32 color);
ui32 terminal_getpixel(unsigned int x, unsigned int y);
void print_character(unsigned char c);
void print_string(unsigned char *c);
void vprintf(unsigned char *format, va_list args);
void printf(unsigned char *format, ...);
void *terminal_getdev();

#endif