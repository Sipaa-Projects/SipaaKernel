#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <sipaa/skgbi.h>
#include <sipaa/conio.h>
#include <sipaa/logger.h>
#include <sipaa/pmm.h>
#include <sipaa/i686/idt.h>
#include <sipaa/i686/gdt.h>
#include <sipaa/i686/tss.h>
#include <sipaa/i686/ps2.h>
#include <sipaa/i686/vmm.h>
#include <sipaa/kdebug.h>
#include <sipaa/syscall.h>
#include <multiboot/multiboot2.h>
#include <flanterm/backends/fb.h>

#if defined(__i686__)
#define KARCHITECTURE "x86 (32-bit)"
#elif defined(__x86_64__)
#define KARCHITECTURE "x86-64 (64-bit)"
#elif defined(__arm__)
#define KARCHITECTURE "ARM"
#elif defined(__aarch64__)
#define KARCHITECTURE "AArch64"
#elif defined(__PPC__)
#define KARCHITECTURE "PowerPC"
#elif defined(__PPC64__)
#define KARCHITECTURE "PowerPC64"
#else
#define KARCHITECTURE "Unknown Architecture"
#endif

#define KVER "0.1 (Alpha)"
#define KSTRING "SipaaKernel " KVER " compiled for " KARCHITECTURE "\nCopyright (C) Sipaa Projects 2024-present"
#define KWELCOME "Welcome to " KSTRING "\n\nThis software comes without ANY WARRANTY. We aren't responsible of any damage caused by the kernel.\n"

extern void switch_to_user();

void usr_main()
{
    log(LT_INFO, "Userspace", "Welcome to userspace!\n");

    // Make a test syscall
    asm volatile(
        "mov $1, %%eax;"
        "xor %%ebx, %%ebx;"
        "int $0x80;"
        :
        :
        : "%eax", "%ebx"
    );

    while (1)
    {
        // asm("hlt"); //Don't do any Ring 0 operations in Ring 3. It's sure at 361% that the CPU will release a GPF out of his garage.
    }
}

void test_paging()
{
    page_directory *dir = create_page_directory();

    enable_paging(dir);

    uintptr_t virtual_addr = 0x400000;
    uintptr_t physical_addr = (uintptr_t)pmm_alloc(PAGE_SIZE);
    map_page(dir, virtual_addr, physical_addr, true, true);

    volatile uint64_t *value = (uint64_t *)virtual_addr;
    *value = 42;
    disable_paging();
}

void kstart(sk_general_boot_info skgbi)
{
    conio_initialize(skgbi);

    conio_println(KWELCOME);

    dbg_init(0x3F8);

    pmm_init(skgbi);

    gdt_init();
    tss_init();
    idt_init();

    ps2kbd_init();
    ps2ms_init();

    // test_paging();

    syscall_init();
    switch_to_user();

    while (1)
    {
    }
}