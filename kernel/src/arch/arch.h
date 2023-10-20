#ifndef __ARCH_H__
#define __ARCH_H__

#include <stdint.h>

#ifdef __x86_64__
extern uint64_t kernel_stack[8192];
#endif

void arch_init_stage1();
void arch_idle();
void arch_shutdown();
void arch_reboot();

#endif