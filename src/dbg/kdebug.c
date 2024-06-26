#include <sipaa/kdebug.h>

#include <sipaa/x86_64/io.h>
#include <sipaa/pmm.h>
#include <nanoprintf/printf.h>
#include <sipaa/logger.h>

unsigned short current_sp;
bool debugger_ready = false;

bool isPortConnected(uint16_t port)
{
    #ifdef __x86_64__
    // Read the Line Status Register
    uint8_t status = inb(port + 5);

    // Check if Data Ready and Empty Transmitter bits are set
    return (status & 0x01) && (status & 0x20);
    #else
    return 0;
    #endif
}

bool Dbg_Initialize(unsigned short sp)
{
    #ifdef __x86_64__
    // Initialize the serial port
    outb(sp + 1, 0x00); // Disable all interrupts
    outb(sp + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(sp + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(sp + 1, 0x00); //                  (hi byte)
    outb(sp + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(sp + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(sp + 4, 0x0B); // IRQs enabled, RTS/DSR set
    outb(sp + 4, 0x1E); // Set in loopback mode, test the serial chip
    outb(sp + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if (inb(sp + 0) != 0xAE)
    {
        return false;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(sp + 4, 0x0F);

    current_sp = sp;
    debugger_ready = true;
    
    return true;
    #else
    // On AArch64, we would use UART to write text.
    debugger_ready = false;
    return false;
    #endif
}

#pragma region Serial IO

char Dbg_ReadTickCheck(int maxTicks)
{
    #ifdef __x86_64__
    int ticks = 0;

    while ((inb(current_sp + 5) & 1) == 0)
    {
        if (ticks >= maxTicks)
            return '\0';
        ticks++;
    }

    return inb(current_sp);
    #endif
}

char Dbg_Read()
{
    #ifdef __x86_64__
    if (debugger_ready)
    {
        while ((inb(current_sp + 5) & 1) == 0)
            ;
        ;

        return inb(current_sp);
    }
    #endif
    return 'h';
}

void serial_putc(char a)
{
    #ifdef __x86_64__
    if (debugger_ready)
    {
        while ((inb(current_sp + 5) & 0x20) == 0)
            ;
        ;

        outb(current_sp, a);
    }
    #endif
}

void Dbg_Print(char *format)
{
    #ifdef __x86_64__
    for (int i = 0; format[i] != '\0'; i++)
    {
        serial_putc(format[i]);
    }
    #endif
}

void Dbg_PrintF(char *format, ...)
{
    #ifdef __x86_64__
    va_list args;
    va_start(args, format);

    char buf[32767];

    npf_vsnprintf(buf, sizeof(buf), format, args);

    Dbg_Print(buf);

    va_end(args);
    #endif
}

void dbg_vprintf(char *format, va_list args)
{
    #ifdef __x86_64__
    char buf[32767];

    npf_vsnprintf(buf, sizeof(buf), format, args);

    Dbg_Print(buf);
    #endif
}

#pragma endregion

void debug_sh()
{
    while (1)
    {
        Dbg_Read();
    }
}

void Dbg_SystemPanic()
{
    if (debugger_ready)
    {
        Dbg_Print("--- !!!! SYSTEM FAILURE !!!! ---\nSipaaKernel is about to crash.\n    - Ignore (i) (will show BSOD)\n    - Halt (h) (will halt the CPU)\n    - Open debug shell (any other key) (will allow you to check the computer state)\nPlease choose the action to do => ");

        char c = Dbg_Read();
        serial_putc(c);

        if (c == 'i')
            return;
        else if (c == 'h')
            while (1)
            #ifdef __x86_64__
                asm("hlt");
            #else
                asm("wfi");
            #endif
        else
            debug_sh();
    }
}