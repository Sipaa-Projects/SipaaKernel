#include <sipaa/x86_64/idt.h>
#include <sipaa/x86_64/io.h>
#include <sipaa/logger.h>

IdtEntryT idt[256];
uint64_t pit_ticks = 0;

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

static char keymap[128] = {
    [0x02] = '1',
    [0x03] = '2',
    [0x04] = '3',
    [0x05] = '4',
    [0x06] = '5',
    [0x07] = '6',
    [0x08] = '7',
    [0x09] = '8',
    [0x0A] = '9',
    [0x0B] = '0',

    [0x10] = 'q',
    [0x11] = 'w',
    [0x12] = 'e',
    [0x13] = 'r',
    [0x14] = 't',
    [0x15] = 'y',
    [0x16] = 'u',
    [0x17] = 'i',
    [0x18] = 'o',
    [0x19] = 'p',

    [0x1E] = 'a',
    [0x1F] = 's',
    [0x20] = 'd',
    [0x21] = 'f',
    [0x22] = 'g',
    [0x23] = 'h',
    [0x24] = 'j',
    [0x25] = 'k',
    [0x26] = 'l',

    [0x2C] = 'z',
    [0x2D] = 'x',
    [0x2E] = 'c',
    [0x2F] = 'v',
    [0x30] = 'b',
    [0x31] = 'n',
    [0x32] = 'm',

    [0x1C] = '\n',
    [0x39] = ' ',
    [0x0E] = '\b',

};

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

void Idt_Initialize()
{
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

    Idt_PicRemap();

    outb(0x21, 0x00);
    outb(0xa1, 0x00);

    Idt_Load();
}

/**void save_state(registers_t *regs)
{
    if (current_process != NULL)
    {
        memcpy(&current_process->regs, regs, sizeof(registers_t));
    }
}

void restore_state(registers_t *regs)
{
    if (current_process != NULL)
    {
        memcpy(regs, &current_process->regs, sizeof(registers_t));
    }
}**/

void general_interrupt_handler(RegistersT *regs)
{
    if (regs->int_no < 32)
    {
        Log(LT_ERROR, "CPU", "Exception: %s\n", exception_messages[regs->int_no]);
        Log(LT_ERROR, "CPU", "Interrupt number: %d\n", regs->int_no);
        Log(LT_ERROR, "CPU", "Error code: %d\n", regs->err_code);
        Log(LT_ERROR, "CPU", "RIP: %x\n", regs->rip);
        Log(LT_ERROR, "CPU", "CS: %x\n", regs->cs);
        Log(LT_ERROR, "CPU", "RFLAGS: %x\n", regs->rflags);
        Log(LT_ERROR, "CPU", "RSP: %x\n", regs->rsp);
        Log(LT_ERROR, "CPU", "SS: %x\n", regs->ss);

        while (1)
            ;
    }
    else
    {
        /**if (regs->int_no == 32)
        {
            tick++;
            save_state(regs);
            schedule();
            restore_state(regs);

            pic_eoi();
        }
        else if (regs->int_no == 33)
        {
            uint8_t scancode = inb(0x60);
            if (scancode < 0x80)
            {
                char key = keymap[scancode];
                if (key)
                {
                    char str[2] = {key, '\0'};
                    printf("%s", str);
                }
            }

            pic_eoi();
        }**/
        Idt_EndOfInterrupt();
    }
}