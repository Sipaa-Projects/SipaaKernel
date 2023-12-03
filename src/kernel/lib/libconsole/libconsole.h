#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdarg.h>
extern int term_x, term_y;

void set_color(uint32_t color);
void print_char(char c);
void printf(const char *format, ...);
void print_animated(const char *message);

#endif