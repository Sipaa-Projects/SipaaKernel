/* Code from osdev (https://wiki.osdev.org/Serial_Ports) */

#include <sk-hal/serial.h>

#define PORT 0x3f8  // COM1

static void outb(uint16_t port, uint8_t data)
{
    __asm__ volatile("outb %0, %1"
                     :
                     : "a"(data), "d"(port));
}

static uint8_t inb(uint16_t port)
{
    uint8_t data;
    __asm__ volatile("inb %1, %0"
                     : "=a"(data)
                     : "d"(port));
    return data;
}

static int is_transmit_empty(void) 
{
    return inb(PORT + 5) & 0x20;
}

void serial_init(void) 
{
    outb(PORT + 1, 0x00);    // Disable all interrupts
    outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(PORT + 1, 0x00);    //                  (hi byte)
    outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
    if(inb(PORT + 0) != 0xAE) 
    {
        return;
    }
 
    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(PORT + 4, 0x0F);
}
 
void serial_putc(char a) 
{
    while (is_transmit_empty() == 0);
    outb(PORT,a);
}

void serial_puts(const char *s)
{
    while (*s)
    {
        serial_putc(*s++);
    }
}