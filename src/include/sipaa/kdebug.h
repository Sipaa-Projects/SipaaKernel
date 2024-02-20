#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdbool.h>

extern bool debugger_ready;

typedef enum SerialPort
{
    Com1 = 0x3F8,
} SerialPort;

bool dbg_init(unsigned short sp);
void dbg_system_panic();
char dbg_read_tickcheck(int maxTicks);
char dbg_read();
void dbg_print(char *format);
void dbg_printf(char *format, ...);

#endif