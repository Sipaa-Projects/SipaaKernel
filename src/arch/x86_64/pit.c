#include <sipaa/x86_64/pit.h>
#include <sipaa/x86_64/idt.h>
#include <sipaa/process.h>

uint32_t tick = 0;

void Pit_Handler(RegistersT *regs)
{
    tick++;

    Schedule(regs);
}

void Pit_Initialize(uint32_t frequency)
{
    uint32_t divisor = PIT_FREQUENCY / frequency;
    outb(0x43, 0x34);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));

    Idt_SetIsrHandler(32, Pit_Handler);
}

void Sleep(uint32_t seconds)
{
    uint32_t eticks = tick + seconds * HZ;
    while (tick < eticks)
    {
        __asm__ __volatile__("hlt");
    }
}

void SleepMs(uint32_t milliseconds)
{
    uint32_t eticks = tick + (milliseconds * HZ) / 1000;
    while (tick < eticks)
    {
        __asm__ __volatile__("hlt");
    }
}

// todo: unistd: add usleep function
void SleepUs(uint32_t milliseconds)
{
    uint32_t eticks = tick + (milliseconds * HZ);
    while (tick < eticks)
    {
        __asm__ __volatile__("hlt");
    }
}
