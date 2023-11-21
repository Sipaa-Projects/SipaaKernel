#ifndef __IO_H
#define __IO_H

#include <stdint.h>

__attribute__((no_caller_saved_registers)) void outb(unsigned short port, unsigned char value);
__attribute__((no_caller_saved_registers)) unsigned char inb(unsigned short port);
__attribute__((no_caller_saved_registers)) void insl(uint16_t port, void *addr, int cnt);
__attribute__((no_caller_saved_registers)) void outw(unsigned short port, unsigned short value);
__attribute__((no_caller_saved_registers)) void outsw(uint16_t port, const void *addr, uint32_t count);
__attribute__((no_caller_saved_registers)) unsigned short inw(unsigned short port);
__attribute__((no_caller_saved_registers)) void insw(uint16_t port, void *addr, int cnt);
__attribute__((no_caller_saved_registers)) void io_wait();
__attribute__((no_caller_saved_registers)) void outl(uint16_t port, uint32_t value);
__attribute__((no_caller_saved_registers)) uint32_t inl(uint16_t port);

#endif