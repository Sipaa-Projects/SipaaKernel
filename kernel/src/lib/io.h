#ifndef __IO_H__
#define __IO_H__

#include <lib/stdtype.h>

void outb(ui16 port, ui8 value);
void outw(ui16 port, ui16 value);
void outsw(ui16 port, const void *addr, ui32 count);
void outl(ui16 port, ui32 value);
ui8 inb(ui16 port);
ui16 inw(ui16 port);
ui32 inl(ui16 port);
void insw(ui16 port, void *addr, int cnt);
void io_wait();

#endif