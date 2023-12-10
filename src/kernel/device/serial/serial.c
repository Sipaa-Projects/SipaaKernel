// SKB_X86_64_ONLY

#include "serial.h"

void init_serial(void)
{
    outb(SERIAL_PORT + 1, 0x00);
    outb(SERIAL_PORT + 3, 0x80);
    outb(SERIAL_PORT + 0, 0x01);
    outb(SERIAL_PORT + 1, 0x00);
    outb(SERIAL_PORT + 3, 0x03);
    outb(SERIAL_PORT + 2, 0xC7);
    outb(SERIAL_PORT + 4, 0x0B);
}

bool serial_is_ready(void)
{
    return inb(SERIAL_PORT + 5) & 0x20;
}

void serial_putc(char c)
{
    while (!serial_is_ready())
        ;
    outb(SERIAL_PORT, c);
}

void serial_putsf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    char buffer[256];

    for (const char *p = format; *p != '\0'; p++)
    {
        if (*p != '%')
        {
            serial_putc(*p);
            continue;
        }

        switch (*++p)
        {
        case 'c':
            serial_putc(va_arg(args, int));
            break;
        case 'd':
            itoa(va_arg(args, int), buffer, 10);
            for (char *p = buffer; *p != '\0'; p++)
                serial_putc(*p);
            break;
        case 'u':
            utoa(va_arg(args, unsigned int), buffer, 10);
            for (char *p = buffer; *p != '\0'; p++)
                serial_putc(*p);
            break;
        case 'x':
            utoa(va_arg(args, unsigned int), buffer, 16);
            for (char *p = buffer; *p != '\0'; p++)
                serial_putc(*p);
            break;
        case 's':
            for (char *p = va_arg(args, char *); *p != '\0'; p++)
                serial_putc(*p);
            break;
        case 'l':
            switch (*++p)
            {
            case 'l':
                switch (*++p)
                {
                case 'x':
                    ullitoa(va_arg(args, unsigned long long int), buffer, 16);
                    for (char *p = buffer; *p != '\0'; p++)
                        serial_putc(*p);
                    break;
                }
                break;
            case 'd':
                itoa(va_arg(args, long int), buffer, 10);
                for (char *p = buffer; *p != '\0'; p++)
                    serial_putc(*p);
                break;
            case 'u':
                utoa(va_arg(args, unsigned long int), buffer, 10);
                for (char *p = buffer; *p != '\0'; p++)
                    serial_putc(*p);
                break;
            case 'x':
                utoa(va_arg(args, unsigned long int), buffer, 16);
                for (char *p = buffer; *p != '\0'; p++)
                    serial_putc(*p);
                break;
            }
            break;

        case 'p':
            utoa(va_arg(args, uintptr_t), buffer, 16);
            for (char *p = buffer; *p != '\0'; p++)
                serial_putc(*p);
            break;
        }
    }

    va_end(args);
}

void serial_puts(const char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        serial_putc(str[i]);
    }
}