#ifndef __SYSCALL_H
#define __SYSCALL_H

#include <stdint.h>
#include <stddef.h>
#include <sipaa/i686/isr.h>

#define SYSCALL_TABLE_LENGTH 32767

typedef void (*SYSCALL_F)(REGISTERS *);

void register_syscall(int id, SYSCALL_F handler);
int syscall_init();

#endif