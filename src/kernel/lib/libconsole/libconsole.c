#include <video/video.h>
#include <stdint.h>
#include <lib/liblogging/liblogging.h>
#include <lib/libc/stdlib.h>

uint32_t term_color = 0xffffff;
int term_x, term_y;

void set_color(uint32_t color)
{
    term_color = color;
}

void scroll_up()
{
    for (int y = 0; y < current_framebuffer->height - 16; y++)
    {
        for (int x = 0; x < current_framebuffer->width; x++)
        {
            uint32_t color = get_pixel(x, y + 16);
            draw_pixel(x, y, color);
        }
    }
    draw_rectangle(0, current_framebuffer->height - 16, current_framebuffer->width, 16, 0x000000);
}

void print_char(char c)
{
    if (usedoublebuffering)
    {
        log(LOG_ERROR, "print_char() isn't available in double buffering.\n");
        return;
    }
    if (c == '\n' || term_x >= current_framebuffer->width)
    {
        term_x = 0;
        term_y += 16;
    }

    if (term_y >= current_framebuffer->height)
    {
        scroll_up();
        term_y -= 16;
    }

    if (c != '\n')
    {
        draw_char(c, term_x, term_y, term_color);
        term_x += 8;
    }
}

void printf(const char *format, ...)
{
    if (usedoublebuffering)
    {
        log(LOG_ERROR, "printf() isn't available in double buffering.\n");
        return;
    }
    va_list args;
    va_start(args, format);

    char buffer[256];

    for (const char *p = format; *p != '\0'; p++)
    {
        if (*p != '%')
        {
            print_char(*p);
            continue;
        }

        switch (*++p)
        {
        case 'c':
            print_char(va_arg(args, int));
            break;
        case 'd':
            itoa(va_arg(args, int), buffer, 10);
            for (char *p = buffer; *p != '\0'; p++)
                print_char(*p);
            break;
        case 'u':
            utoa(va_arg(args, unsigned int), buffer, 10);
            for (char *p = buffer; *p != '\0'; p++)
                print_char(*p);
            break;
        case 'x':
            utoa(va_arg(args, unsigned int), buffer, 16);
            for (char *p = buffer; *p != '\0'; p++)
                print_char(*p);
            break;
        case 's':
            for (char *p = va_arg(args, char *); *p != '\0'; p++)
                print_char(*p);
            break;
        case 'l':
            switch (*++p)
            {
            case 'l':
                switch (*++p)
                {
                case 'u':
                    ullitoa(va_arg(args, unsigned long long int), buffer, 10);
                    for (char *p = buffer; *p != '\0'; p++)
                        print_char(*p);
                    break;
                case 'x':
                    ullitoa(va_arg(args, unsigned long long int), buffer, 16);
                    for (char *p = buffer; *p != '\0'; p++)
                        print_char(*p);
                    break;
                }
                break;
            case 'd':
                itoa(va_arg(args, long int), buffer, 10);
                for (char *p = buffer; *p != '\0'; p++)
                    print_char(*p);
                break;
            case 'u':
                utoa(va_arg(args, unsigned long int), buffer, 10);
                for (char *p = buffer; *p != '\0'; p++)
                    print_char(*p);
                break;
            case 'x':
                utoa(va_arg(args, unsigned long int), buffer, 16);
                for (char *p = buffer; *p != '\0'; p++)
                    print_char(*p);
                break;
            }
            break;

        case 'p':
            utoa(va_arg(args, uintptr_t), buffer, 16);
            for (char *p = buffer; *p != '\0'; p++)
                print_char(*p);
            break;
        }
    }

    va_end(args);
}

void print_animated(const char *message)
{
    if (usedoublebuffering)
    {
        log(LOG_ERROR, "print_animated() isn't available in double buffering.\n");
        return;
    }
    uint32_t color = 0x0000FF; 

    while (*message)
    {
        set_color(color);

        print_char(*message++);
        delay(10);

        color += 0x001100;
        if (color > 0x00FF00)
            color = 0x0000FF;
    }

    set_color(0xFFFFFF);
}