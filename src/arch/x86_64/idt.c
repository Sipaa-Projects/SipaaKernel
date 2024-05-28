#include <sipaa/x86_64/idt.h>
#include <sipaa/x86_64/io.h>
#include <sipaa/x86_64/vmm.h>
#include <sipaa/x86_64/pit.h>
#include <sipaa/libc/string.h>
#include <sipaa/sched.h>
#include <sipaa/heap.h>
#include <sipaa/logger.h>
#include <sipaa/kdebug.h>
#include <stddef.h>

IdtEntryT idt[256];
IsrHandler isrh[256]; 
uint64_t pit_ticks = 0;
bool Idt_PendingReboot = false;

extern void Idt_Load();

static char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"};

void Idt_PicRemap()
{
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 8);
    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

void Idt_EndOfInterrupt()
{
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}

void Idt_SetEntry(int i, uint64_t base, uint16_t sel, uint8_t flags)
{
    idt[i].offset_low = (uint16_t)(base & 0xFFFF);
    idt[i].offset_mid = (uint16_t)((base >> 16) & 0xFFFF);
    idt[i].offset_high = (uint32_t)(base >> 32);
    idt[i].selector = sel;
    idt[i].ist = 0;
    idt[i].types_attr = flags;
    idt[i].zero = 0;
}

void Idt_PrepareReboot() { Idt_PendingReboot = true; }

void Idt_Initialize()
{
    for (int i = 0; i < 256; i++)
    {
        isrh[i] = NULL;
    }

    Idt_SetEntry(0, (uint64_t)&isr0, 0x08, 0x8E);
    Idt_SetEntry(1, (uint64_t)&isr1, 0x08, 0x8E);
    Idt_SetEntry(2, (uint64_t)&isr2, 0x08, 0x8E);
    Idt_SetEntry(3, (uint64_t)&isr3, 0x08, 0x8E);
    Idt_SetEntry(4, (uint64_t)&isr4, 0x08, 0x8E);
    Idt_SetEntry(5, (uint64_t)&isr5, 0x08, 0x8E);
    Idt_SetEntry(6, (uint64_t)&isr6, 0x08, 0x8E);
    Idt_SetEntry(7, (uint64_t)&isr7, 0x08, 0x8E);
    Idt_SetEntry(8, (uint64_t)&isr8, 0x08, 0x8E);
    Idt_SetEntry(9, (uint64_t)&isr9, 0x08, 0x8E);
    Idt_SetEntry(10, (uint64_t)&isr10, 0x08, 0x8E);
    Idt_SetEntry(11, (uint64_t)&isr11, 0x08, 0x8E);
    Idt_SetEntry(12, (uint64_t)&isr12, 0x08, 0x8E);
    Idt_SetEntry(13, (uint64_t)&isr13, 0x08, 0x8E);
    Idt_SetEntry(14, (uint64_t)&isr14, 0x08, 0x8E);
    Idt_SetEntry(16, (uint64_t)&isr16, 0x08, 0x8E);
    Idt_SetEntry(17, (uint64_t)&isr17, 0x08, 0x8E);
    Idt_SetEntry(18, (uint64_t)&isr18, 0x08, 0x8E);
    Idt_SetEntry(19, (uint64_t)&isr19, 0x08, 0x8E);
    Idt_SetEntry(20, (uint64_t)&isr20, 0x08, 0x8E);
    Idt_SetEntry(21, (uint64_t)&isr21, 0x08, 0x8E);
    Idt_SetEntry(22, (uint64_t)&isr22, 0x08, 0x8E);
    Idt_SetEntry(23, (uint64_t)&isr23, 0x08, 0x8E);
    Idt_SetEntry(24, (uint64_t)&isr24, 0x08, 0x8E);
    Idt_SetEntry(25, (uint64_t)&isr25, 0x08, 0x8E);
    Idt_SetEntry(26, (uint64_t)&isr26, 0x08, 0x8E);
    Idt_SetEntry(27, (uint64_t)&isr27, 0x08, 0x8E);
    Idt_SetEntry(28, (uint64_t)&isr28, 0x08, 0x8E);
    Idt_SetEntry(29, (uint64_t)&isr29, 0x08, 0x8E);
    Idt_SetEntry(30, (uint64_t)&isr30, 0x08, 0x8E);
    Idt_SetEntry(31, (uint64_t)&isr31, 0x08, 0x8E);
    Idt_SetEntry(32, (uint64_t)&isr32, 0x08, 0x8E);
    Idt_SetEntry(33, (uint64_t)&isr33, 0x08, 0x8E);
    Idt_SetEntry(34, (uint64_t)&isr34, 0x08, 0x8E);
    Idt_SetEntry(35, (uint64_t)&isr35, 0x08, 0x8E);
    Idt_SetEntry(36, (uint64_t)&isr36, 0x08, 0x8E);
    Idt_SetEntry(37, (uint64_t)&isr37, 0x08, 0x8E);
    Idt_SetEntry(38, (uint64_t)&isr38, 0x08, 0x8E);
    Idt_SetEntry(39, (uint64_t)&isr39, 0x08, 0x8E);
    Idt_SetEntry(40, (uint64_t)&isr40, 0x08, 0x8E);
    Idt_SetEntry(41, (uint64_t)&isr41, 0x08, 0x8E);
    Idt_SetEntry(42, (uint64_t)&isr42, 0x08, 0x8E);
    Idt_SetEntry(43, (uint64_t)&isr43, 0x08, 0x8E);
    Idt_SetEntry(44, (uint64_t)&isr44, 0x08, 0x8E);
    Idt_SetEntry(45, (uint64_t)&isr45, 0x08, 0x8E);
    Idt_SetEntry(46, (uint64_t)&isr46, 0x08, 0x8E);
    Idt_SetEntry(47, (uint64_t)&isr47, 0x08, 0x8E);
    Idt_SetEntry(48, (uint64_t)&isr48, 0x08, 0x8E);
    Idt_SetEntry(128, (uint64_t)&isr128, 0x08, 0x8E);

    Idt_PicRemap();

    outb(0x21, 0x00);
    outb(0xa1, 0x00);

    Idt_Load();
    //asm("sti");
}

void Idt_SetIsrHandler(int vector, IsrHandler isr)
{
    // The Idt_SetIsrHandler function does not allow setting ISRs for exceptions.
    // (btw if it was allowed, it still won't run the handler since the panic handler is integrated)
    if (vector < 32)
        return;

    // Do NOT overwrite non-null handlers
    if (isrh[vector] != NULL)
        return;

    // Now, we can set the handler.
    isrh[vector] = isr;
}

void Idt_SaveState(RegistersT *regs)
{
    if (current_process != NULL)
    {
        memcpy(&current_process->regs, regs, sizeof(RegistersT));
    }
}

void Idt_RestoreState(RegistersT *regs)
{
    if (current_process != NULL)
    {
        memcpy(regs, &current_process->regs, sizeof(RegistersT));
    }
}

void general_interrupt_handler(RegistersT *regs)
{
    if (regs->int_no != 32 && !Idt_PendingReboot)
        Log(LT_DEBUG, "CPU", "Received interrupt %u\n", regs->int_no);

    if (regs->int_no < 32 && !Idt_PendingReboot)
    {
        if (regs->int_no == 14)
        {
            if (vmm_current_address_space == current_process->pml4)
            {
                int pid = current_process->pid;

                Scheduler_ExitProcess(current_process);

                Log(LT_ERROR, "CPU", "Process %d exited due to a segmentation fault... At least, i didn't panic!\n", pid);

                asm("int $32");
                return;
            }
        }

        Dbg_SystemPanic();

        Log(LT_FATAL, "CPU", "Exception: %s\n", exception_messages[regs->int_no]);
        Log(LT_FATAL, "CPU", "Interrupt number: %d\n", regs->int_no);
        Log(LT_FATAL, "CPU", "Error code: %d\n", regs->err_code);
        Log(LT_FATAL, "CPU", "RIP: %x\n", regs->rip);
        Log(LT_FATAL, "CPU", "CS: %x\n", regs->cs);
        Log(LT_FATAL, "CPU", "RFLAGS: %x\n", regs->rflags);
        Log(LT_FATAL, "CPU", "RSP: %x\n", regs->rsp);
        Log(LT_FATAL, "CPU", "SS: %x\n", regs->ss);

        while (1)
            ;
    }
    else
    {
        if (isrh[regs->int_no] != NULL)
            isrh[regs->int_no](regs);

        Idt_EndOfInterrupt();
    }
}