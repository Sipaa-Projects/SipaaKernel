#if defined(__x86_64__) | defined(__i686__) | defined(__i386__)

#include "serial.h"
#include <arch/x86/io.h>

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

int serial_received() 
{
    return inb(SERIAL_PORT + 5) & 1;
}
 
char serial_readc() {
   while (serial_received() == 0);
 
   return inb(SERIAL_PORT);
}

void serial_putc(char c)
{
    while (!serial_is_ready())
        ;
    outb(SERIAL_PORT, c);
}

void serial_puts(const char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        serial_putc(str[i]);
    }
}

#endif