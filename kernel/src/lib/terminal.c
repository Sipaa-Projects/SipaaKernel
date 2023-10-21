#include <lib/terminal.h>
#include <dev/device.h>
#include "psf.h"

Terminal * current_terminal;
Terminal * main_terminal;

void init_terminal(Terminal * terminal, ui8 * font, framebuffer * framebuffer) {
    terminal -> font = font;
    terminal -> framebuffer = framebuffer;
    terminal -> cursor_x = 0;
    terminal -> cursor_y = 0;
    terminal -> bgcolor = 0x0;
    terminal -> fgcolor = 0xFFFFFF;
    current_terminal = terminal;
}

void switch_terminal(Terminal * terminal) {
    current_terminal = terminal;
}

void clear_terminal(ui32 color) {
    unsigned int * pixel = current_terminal -> framebuffer -> address;
    for (unsigned int y = 0; y < current_terminal -> framebuffer -> height; ++y) {
        for (unsigned int x = 0; x < current_terminal -> framebuffer -> width; ++x) {
            * pixel++ = color;
        }
        pixel += (current_terminal -> framebuffer -> pitch / 4) - current_terminal -> framebuffer -> width;
    }
}

void terminal_putpixel(unsigned int x, unsigned int y, ui32 color) {
    unsigned int * pixel = current_terminal -> framebuffer -> address + (y * (current_terminal -> framebuffer -> pitch / 4) + x);
    * pixel = color;
}

ui32 terminal_getpixel(unsigned int x, unsigned int y) {
    unsigned int * pixel = current_terminal -> framebuffer -> address + (y * (current_terminal -> framebuffer -> pitch / 4) + x);
    return * pixel;
}

void scroll_up() {
    for (int y = 0; y < current_terminal -> framebuffer -> height - 16; y++) {
        for (int x = 0; x < current_terminal -> framebuffer -> width; x++) {
            ui32 color = terminal_getpixel(x, y + 16);
            terminal_putpixel(x, y, color);
        }
    }

    for (int i = 0; i < 0 + current_terminal -> framebuffer -> width; i++) {
        for (int j = current_terminal -> framebuffer -> height - 15; j < current_terminal -> framebuffer -> height - 16 + 16; j++) {
            terminal_putpixel(i, j, 0x00000000);
        }
    }
}

void print_character(unsigned char c) {
    if (c == '\n') {
        current_terminal -> cursor_x = 0;
        current_terminal -> cursor_y += 16;
    }
    if (current_terminal -> cursor_x >= current_terminal -> framebuffer -> width) {
        current_terminal -> cursor_x = 0;
        current_terminal -> cursor_y += 16;
    }

    if (current_terminal -> cursor_y >= current_terminal -> framebuffer -> height) {
        scroll_up();
        current_terminal -> cursor_y -= 16;
    }

    if (c != '\n') {
        ui8 * offset = current_terminal -> font + sizeof(PSF1_HEADER) + 16 * c;
        
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 8; j++) {
                if (offset[i] & (1 << j)) {
                    terminal_putpixel(current_terminal -> cursor_x + 8 - j, current_terminal -> cursor_y + i, current_terminal -> fgcolor);
                }
            }
        }
        current_terminal -> cursor_x += 8;
    }
}

void print_string(unsigned char * str) {
    for (int i = 0; str[i] != '\0'; i++) {
        print_character(str[i]);
    }
}

void print_integer(int v, int base,
    unsigned char * digits) {
    char buf[33];
    char * ptr = & buf[sizeof(buf) - 1];
    * ptr = '\0';

    if (v == 0) {
        print_character('0');
        return;
    }

    if (v < 0 && base == 10) {
        print_character('-');
        v = -v;
    }

    while (v) {
        *--ptr = digits[v % base];
        v /= base;
    }

    print_string(ptr);
}

void print_integer_64(uint64_t v, int base,
    unsigned char * digits) {
    char buf[65];
    char * pointer = & buf[sizeof(buf) - 1];
    * pointer = '\0';

    if (v == 0) {
        print_character('0');
        return;
    }

    while (v) {
        *--pointer = digits[v % base];
        v /= base;
    }

    print_string(pointer);
}

void vprintf(unsigned char *format, va_list args)
{
        const char *hex_digits = "0123456789ABCDEF";

        while (*format != '\0')
        {
                if (*format == '%')
                {
                        format++;
                        switch (*format)
                        {
                        case 's':
                        {
                                char *str = va_arg(args, char *);
                                print_string(str);
                                break;
                        }
                        case 'c':
                        {
                                char c = (char)va_arg(args, int);
                                print_character(c);
                                break;
                        }
                        case 'd':
                        {
                                int d = va_arg(args, int);
                                print_integer(d, 10, hex_digits);
                                break;
                        }
                        case 'u':
                        {
                                unsigned int u = va_arg(args, unsigned int);
                                print_integer(u, 10, hex_digits);
                                break;
                        }
                        case 'x':
                        {
                                int x = va_arg(args, int);
                                print_integer(x, 16, hex_digits);
                                break;
                        }
                        case 'p':
                        {
                                void *p = va_arg(args, void *);
                                print_integer_64((uint64_t)p, 16, hex_digits);
                                break;
                        }
                        case 'l':
                        {
                                format++;
                                if (*format == 'l')
                                {
                                        format++;
                                        if (*format == 'u')
                                        {
                                                uint64_t llu = va_arg(args, uint64_t);
                                                print_integer_64(llu, 10, hex_digits);
                                        }
                                        else if (*format == 'x')
                                        {
                                                uint64_t llx = va_arg(args, uint64_t);
                                                print_integer_64(llx, 16, hex_digits);
                                        }
                                        else
                                        {
                                                print_string("Invalid format specifier");
                                        }
                                }
                                else
                                {
                                        print_string("Invalid format specifier");
                                }
                                break;
                        }
                        default:
                                print_character('%');
                                print_character(*format);
                                break;
                        }
                }
                else
                {
                        print_character(*format);
                }
                format++;
        }
}

void printf(unsigned char *format, ...)
{
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
}

/* Device */
void terminal_write(dev *self, const char *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        print_character(buf[i]);
    }
}

void terminal_read(dev *self, char *buf, size_t len) {
    // Reading from serial console can't be done.
    buf = NULL;
    return;
}

void *terminal_getdev()
{
    void *d = malloc(sizeof(dev));

    if (!d)
    {
        return;
        //log(LOGTYPE_WARNING, "serial_init: malloc returned NULL. You may encounter problems later.");
    }

    dev *d2 = (dev *)d;
    d2->write = &terminal_write;
    d2->read = &terminal_read;

    return d;
}