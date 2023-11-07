#ifndef __PIC_H
#define __PIC_H

#include "io.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

__attribute__((no_caller_saved_registers)) void pic_end_master();
__attribute__((no_caller_saved_registers)) void pic_end_slave();
void remap_pic();

#endif