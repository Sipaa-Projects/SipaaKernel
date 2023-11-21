#include "io.h"

__attribute__((no_caller_saved_registers)) void outb(unsigned short port, unsigned char value)
{
    asm volatile("outb %0, %1"
                 :
                 : "a"(value), "Nd"(port));
}

__attribute__((no_caller_saved_registers)) unsigned char inb(unsigned short port)
{
    unsigned char result;
    asm volatile("inb %1, %0"
                 : "=a"(result)
                 : "Nd"(port));
    return result;
}

__attribute__((no_caller_saved_registers)) void insl(uint16_t port, void *addr, int cnt)
{
    asm volatile("cld; rep insl"
                 : "=D"(addr), "=c"(cnt)
                 : "d"(port), "0"(addr), "1"(cnt)
                 : "memory", "cc");
}

__attribute__((no_caller_saved_registers)) void outw(unsigned short port, unsigned short value)
{
    asm volatile("outw %0, %1"
                 :
                 : "a"(value), "Nd"(port));
}

__attribute__((no_caller_saved_registers)) void outsw(uint16_t port, const void *addr, uint32_t count)
{
    asm volatile("cld; rep outsw"
                 : "+S"(addr), "+c"(count)
                 : "d"(port));
}

__attribute__((no_caller_saved_registers)) unsigned short inw(unsigned short port)
{
    unsigned short result;
    asm volatile("inw %1, %0"
                 : "=a"(result)
                 : "Nd"(port));
    return result;
}

__attribute__((no_caller_saved_registers)) void insw(uint16_t port, void *addr, int cnt)
{
    asm volatile("cld; rep insw"
                 : "=D"(addr), "=c"(cnt)
                 : "d"(port), "0"(addr), "1"(cnt)
                 : "memory", "cc");
}

__attribute__((no_caller_saved_registers)) void io_wait()
{
    asm volatile("outb %%al, $0x80"
                 :
                 : "a"(0));
}

__attribute__((no_caller_saved_registers)) void outl(uint16_t port, uint32_t value) {
    asm volatile ("outl %0, %1" : : "a"(value), "Nd"(port));
}

__attribute__((no_caller_saved_registers)) uint32_t inl(uint16_t port) {
    uint32_t value;
    asm volatile ("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}