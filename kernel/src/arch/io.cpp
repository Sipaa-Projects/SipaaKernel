#ifndef __aarch64__
#include "io.h"

namespace Sk {
namespace Arch {

void Io::OutB(uint16_t port, uint8_t value)
{
    asm volatile("outb %0, %1"
                 :
                 : "a"(value), "Nd"(port));
}
void Io::OutW(uint16_t port, uint16_t value)
{
    asm volatile("outw %0, %1"
                 :
                 : "a"(value), "Nd"(port));
}
void Io::OutSW(uint16_t port, const void *addr, uint32_t count)
{
    asm volatile("cld; rep outsw"
                 : "+S"(addr), "+c"(count)
                 : "d"(port));
}

void Io::OutL(uint16_t port, uint32_t value)
{
    asm volatile ("outl %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t Io::InB(uint16_t port)
{
    unsigned char result;
    asm volatile("inb %1, %0"
                 : "=a"(result)
                 : "Nd"(port));
    return result;
    
}
uint16_t Io::InW(uint16_t port)
{
    unsigned short result;
    asm volatile("inw %1, %0"
                 : "=a"(result)
                 : "Nd"(port));
    return result;
}

uint32_t Io::InL(uint16_t port)
{
    uint32_t value;
    asm volatile ("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void Io::InSW(uint16_t port, void *addr, int cnt)
{
    asm volatile("cld; rep insw"
                 : "=D"(addr), "=c"(cnt)
                 : "d"(port), "0"(addr), "1"(cnt)
                 : "memory", "cc");
}

void Io::Wait()
{
    asm volatile("outb %%al, $0x80"
                 :
                 : "a"(0));
}

} // namespace Arch
} // namespace Sk
#endif