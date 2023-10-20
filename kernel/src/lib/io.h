#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>

void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);
void outsw(uint16_t port, const void *addr, uint32_t count);
void outl(uint16_t port, uint32_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);
void insw(uint16_t port, void *addr, int cnt);
void io_wait();

#endif