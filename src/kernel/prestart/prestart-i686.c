// SKB_i686_ONLY

#include <entry/multiboot.h>
#include <entry/entry.h>
#include <logger/logger.h>
#include <stdnoreturn.h>
#include "sk_prestart_return.h"

extern int _start(sk_general_boot_info skgbi);

noreturn void prestart_stage2(struct multiboot_info *info)
{
    sk_general_boot_info skgbi = get_skgbi_from_multiboot(info);

    int kr = _start(skgbi);

    if (kr == SK_RETURN_LOOP)
    {
        log(LT_INFO, "prestart", "kernel returned %d (aka SK_RETURN_LOOP). entering loop", kr);
        while (1)
        {

        }
    }
    else if (kr == SK_RETURN_HLT)
    {
        log(LT_INFO, "prestart", "kernel returned %d (aka SK_RETURN_HLT). halting system.", kr);
        while (1)
        {
            #if defined(__RISCV64__) | defined(__AARCH64__)
            asm("wfi");
            #elif defined(__x86_64__)
            asm("hlt");
            #endif
        }
    }
    else if (kr == SK_RETURN_CLI_HLT)
    {
        log(LT_INFO, "prestart", "kernel returned %d (aka SK_RETURN_CLI_HLT). disabling ints and halting system.", kr);
        #ifdef __x86_64__
        asm("cli");
        #endif
        while (1)
        {
            #if defined(__RISCV64__) | defined(__AARCH64__)
            asm("wfi");
            #elif defined(__x86_64__)
            asm("hlt");
            #endif
        }
    }
}