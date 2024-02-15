#ifndef IO_H
#define IO_H

#include <stdint.h>

void outb(unsigned short port, unsigned char value);
unsigned char inb(unsigned short port);
void outl(uint16_t port, uint32_t val);
uint32_t inl(uint16_t port);
void insl(uint16_t port, void *addr, int cnt);
void outw(unsigned short port, unsigned short value);
void outsw(uint16_t port, const void *addr, uint32_t count);
unsigned short inw(unsigned short port);
void insw(uint16_t port, void *addr, int cnt);
void io_wait();

#endif