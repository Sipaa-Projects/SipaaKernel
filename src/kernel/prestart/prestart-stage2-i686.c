// SKB_i686_ONLY

#include <entry/multiboot2.h>
#include <entry/entry.h>
#include <logger/logger.h>
#include <stdnoreturn.h>
#include <device/serial/serial.h>
#include "sk_prestart_return.h"

extern int _start(sk_general_boot_info skgbi);

noreturn void prestart_stage2(uint32_t magic, struct multiboot_info* info_addr)
{
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        init_serial();
        serial_puts("[FATAL] prestart: multiboot2 bootloader magic is invalid!\n");
        serial_puts("[FATAL] Please contact the bootloader's developer.\n");
        while (1)
        {
            asm("hlt");
        }
    }

    sk_general_boot_info skgbi = get_skgbi_from_multiboot(info_addr);

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
