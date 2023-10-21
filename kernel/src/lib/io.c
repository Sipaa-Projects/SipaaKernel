#ifdef __x86_64__
#include <lib/io.h>

void outb(ui16 port, ui8 value)
{
    asm volatile("outb %0, %1"
                 :
                 : "a"(value), "Nd"(port));
}

void outw(ui16 port, ui16 value)
{
    asm volatile("outw %0, %1"
                 :
                 : "a"(value), "Nd"(port));
}

void outsw(ui16 port, const void *addr, ui32 count)
{
    asm volatile("cld; rep outsw"
                 : "+S"(addr), "+c"(count)
                 : "d"(port));
}

void outl(ui16 port, ui32 value)
{
    asm volatile ("outl %0, %1" : : "a"(value), "Nd"(port));
}

ui8 inb(ui16 port)
{
    unsigned char result;
    asm volatile("inb %1, %0"
                 : "=a"(result)
                 : "Nd"(port));
    return result;
    
}
ui16 inw(ui16 port)
{
    unsigned short result;
    asm volatile("inw %1, %0"
                 : "=a"(result)
                 : "Nd"(port));
    return result;
}

ui32 inl(ui16 port)
{
    ui32 value;
    asm volatile ("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void insw(ui16 port, void *addr, int cnt)
{
    asm volatile("cld; rep insw"
                 : "=D"(addr), "=c"(cnt)
                 : "d"(port), "0"(addr), "1"(cnt)
                 : "memory", "cc");
}

void io_wait()
{
    asm volatile("outb %%al, $0x80"
                 :
                 : "a"(0));
}
#endif