#include <sipaa/i686/io.h>

/**
 * read a byte from given port number
 */
uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * write a given byte to given port number
 */
void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %1, %0" :: "dN"(port), "a"(val));
}

/**
 * read 2 bytes(short) from given port number
 */
uint16_t ins(uint16_t port) {
    uint16_t rv;
    asm volatile ("inw %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

/**
 * write given 2 bytes(short) to given port number
 */
void outs(uint16_t port, uint16_t data) {
    asm volatile ("outw %1, %0" : : "dN" (port), "a" (data));
}

/**
 * read 4 bytes(long) from given port number
 */
uint32_t inl(uint16_t port) {
    uint32_t rv;
    asm volatile ("inl %%dx, %%eax" : "=a" (rv) : "dN" (port));
    return rv;
}

/**
 * write given 4 bytes(long) to given port number
 */
void outl(uint16_t port, uint32_t data) {
    asm volatile ("outl %%eax, %%dx" : : "dN" (port), "a" (data));
}

void outw(uint16_t port, uint16_t value)
{
    __asm__ volatile("outw %0, %1" : : "a" (value), "d" (port));
}

uint16_t inw(uint16_t port)
{
    uint16_t value;
    __asm__ volatile("inw %1, %0" : "=a" (value) : "d" (port));
    return value;
}