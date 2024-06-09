#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <sipaa/bootsrv.h>
#include <sipaa/dev/bga.h>
#include <sipaa/pmm.h>
#include <sipaa/kdebug.h>
#include <sipaa/logger.h>
#include <sipaa/fs/vfs.h>
#include <sipaa/fs/hellofs.h>
#include <sipaa/fs/ext2.h>
#include <sipaa/x86_64/gdt.h>
#include <sipaa/x86_64/idt.h>
#include <sipaa/x86_64/vmm.h>
#include <sipaa/x86_64/pit.h>
#include <sipaa/bootsrv.h>
#include <sipaa/pci.h>
#include <sipaa/dev/conio.h>
#include <sipaa/framebuffer.h>
#include <sipaa/exec/elf.h>
#include <sipaa/klang.h>
#include <sipaa/heap.h>
#include <sipaa/sched.h>
#include <sipaa/syscall.h>
#include <sipaa/ksym.h>
#include <sipaa/dev/block/ata.h>

void usr_main() {}

uint64_t kernel_stack[8192];

extern void SK_Reboot();

#ifdef __x86_64__
void *execute_syscall(uint64_t num, ...)
{
    void *params[6];

    va_list arg_list;
    va_start(arg_list, num);

    for (int j = 0; j < 6; j++)
    {
        void *param = va_arg(arg_list, void *);
        if (param == NULL)
            params[j] = 0;
        else
            params[j] = param;
    }

    __asm__ volatile(
        "movq %0, %%rax;"
        "movq %1, %%rdi;"
        "movq %2, %%rsi;"
        "movq %3, %%rdx;"
        "movq %4, %%r10;"
        "movq %5, %%r8;"
        "movq %6, %%r9;"
        "int $0x80;"
        :
        : "r"((uint64_t)num), "r"((uint64_t)params[0]), "r"((uint64_t)params[1]), "r"((uint64_t)params[2]), "r"((uint64_t)params[3]), "r"((uint64_t)params[4]), "r"((uint64_t)params[5])
        : "rax", "rdi", "rsi", "rdx", "r10", "r8");

    va_end(arg_list);
    void *result;
    __asm__ volatile("" : "=a"(result) : : "memory");
    return result;
}
#endif

int Task1()
{
    Log(LT_INFO, "Task1", "Hello world from Task1!\n");

    //execute_syscall(0, "Hello, World!\n");

    return 0;
}

void SKEntry()
{
    Dbg_Initialize(Com1);
    Fbuf_Initialize();
    ConIO_Initialize();
    ConIO_Print(KLANG_WELCOMETOSK);

    Log(LT_INFO, "kernel", "starting initialization...\n");

    //BootSrv_EnumerateProtocolInfos();
    #ifdef __x86_64__
    Gdt_Initialize(kernel_stack);
    Idt_Initialize();
    #endif

    Pmm_Initialize();
    KHeap_Initialize();
    #ifdef __x86_64__
    Vmm_Initialize();
    #endif

    KernelSymbols_Initialize();
    Fbuf_InitializeGPU();

    #ifdef __x86_64__
    Syscall_Initialize();
    Scheduler_Initialize();
    Pit_Initialize(1000);

    Scheduler_CreateProcess("Task1", Task1);
    #endif

    Log(LT_SUCCESS, "kernel", "SipaaKernel has been fully initialized! We will now hide logs.\n");

    #ifdef __x86_64__
    asm("sti");
    #endif

    for (;;)
        ;;
}