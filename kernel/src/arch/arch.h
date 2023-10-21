#ifndef __ARCH_H__
#define __ARCH_H__

#include <lib/stdtype.h>

#ifdef __x86_64__
extern ui64 kernel_stack[8192];
#endif

void arch_init_stage1();
void arch_idle();
void arch_shutdown();
void arch_reboot();

#endif