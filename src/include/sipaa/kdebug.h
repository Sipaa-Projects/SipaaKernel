#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdbool.h>

extern bool debugger_ready;

typedef enum SerialPort
{
    Com1 = 0x3F8,
} SerialPort;

bool Dbg_Initialize(unsigned short sp);
void Dbg_SystemPanic();
char Dbg_ReadTickCheck(int maxTicks);
char Dbg_Read();
void Dbg_Print(char *format);
void Dbg_PrintF(char *format, ...);

#endif