#ifndef SERIAL_H
#define SERIAL_H

#include <stddef.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

#define SERIAL_PORT 0x3f8

void init_serial(void);
bool serial_is_ready(void);
void serial_putc(char c);
void serial_puts(const char *str);
void serial_putsf(const char *format, ...);

#endif