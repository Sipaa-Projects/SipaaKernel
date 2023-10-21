#include <arch/arch.h>

#ifdef __x86_64__
#include <arch/x86_64/gdt.h>
#include <arch/x86_64/cpu.h>
#endif
#include <lib/log.h>
#include <stdnoreturn.h>

#ifdef __x86_64__
ui64 kernel_stack[8192];
#endif

void arch_init_stage1(void)
{
    #if defined(__x86_64__)
    log(LOGTYPE_INFO, "arch: cpu arch: x64 x86-based (x86-64)\n");
    log(LOGTYPE_INFO, "arch: initializing for x86_64\n");
    log(LOGTYPE_INFO, "arch: initializing gdt/tss\n");
    gdt_init(kernel_stack);
    log(LOGTYPE_SUCCESS, "arch: initialized gdt/tss\n");
    log(LOGTYPE_INFO, "arch: initializing fpu\n");
    if (sys_init_fpu() == 0)
        log(LOGTYPE_SUCCESS, "arch: initialized fpu\n");
        
    else
        log(LOGTYPE_WARNING, "arch: fpu isn't present, floating point math\n");
    log(LOGTYPE_SUCCESS, "arch: initialized\n");
    #elif defined(__aarch64__) | defined(__riscv64__)
    log(LOGTYPE_INFO, "arch: cpu arch: x64 RISCV-based (RISCV64/AARCH64)\n");
    log(LOGTYPE_INFO, "arch: nothing to initialize\n");
    #endif
}

noreturn void arch_idle(void)
{
    log(LOGTYPE_INFO, "arch: entering idle mode (halting system)\n");
    while (1)
    {
        #if defined(__x86_64__)
        __asm__ volatile("hlt");
        #elif defined(__aarch64__) || defined(__riscv64__)
        __asm__ volatile("wfi");
        #endif 
    }
}

void arch_pause(void) {
    #ifdef __x86_64__
    __asm__ volatile("pause");
    #endif
}

noreturn void arch_reboot(void) {
    // todo
    arch_idle();
}

noreturn void arch_shutdown(void) {
    // todo
    arch_idle();
}