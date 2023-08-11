#include "idt.h"

#include "io.h"

#include <global.h>
#include <dev/serial.h>
#include <dev/ps2.h>
#include <console/console.h>
#include <logging/logger.h>
#include <system/system.h>

using namespace Sk::Logging;

namespace Sk {
namespace Arch {

IdtEntry Idt::entries[256];
IdtPtr Idt::idtr;

__attribute__((interrupt)) void DivZeroHandler(struct Registers64 *r)
{
    System::panic("The CPU tried to do a division by 0.", r);
}
__attribute__((interrupt)) void DebugHandler(struct Registers64 *r)
{
    System::panic("Debug.", r);
}
__attribute__((interrupt)) void NonMaskableInterruptHandler(struct Registers64 *r)
{
    System::panic("The PIC tried to mask a non-maskable interrupt.", r);
}
__attribute__((interrupt)) void BreakPointHandler(struct Registers64 *r)
{
    System::panic("Instruction 'int3' has been raised.", r);
}
__attribute__((interrupt)) void OverflowHandler(struct Registers64 *r)
{
    System::panic("Overflow.", r);
}
__attribute__((interrupt)) void BoundRangeExceeded(struct Registers64 *r)
{
    System::panic("Bound range has been exceeded.", r);
}
__attribute__((interrupt)) void InvalidOpcodeHandler(struct Registers64 *r)
{
    System::panic("The CPU tried run an invalid opcode.", r);
}
__attribute__((interrupt)) void DeviceNotAvailableHandler(struct Registers64 *r)
{
    System::panic("The kernel tried to access a not available device.", r);
}
__attribute__((interrupt)) void DoubleFaultHandler(struct Registers64 *r)
{
    System::panic("Somehow, you managed to get a fault in a fault! Congratulations!", r);
}
__attribute__((interrupt)) void InvalidTssHandler(struct Registers64 *r)
{
    System::panic("Invalid TSS.", r);
}
__attribute__((interrupt)) void SegNotPresentHandler(struct Registers64 *r)
{
    System::panic("Segment not present.", r);
}
__attribute__((interrupt)) void StackFaultHandler(struct Registers64 *r)
{
    System::panic("S\nt\na\nc\nk\n\nf\na\nu\nt\nl\n.", r);
}
__attribute__((interrupt)) void GpfHandler(struct Registers64 *r)
{
    System::panic("Looks like the CPU did go in his bunker because he doesn't like your code...", r);
}
__attribute__((interrupt)) void PfHandler(struct Registers64 *r)
{
    System::panic("Looks like there was a fault with your book's pages...", r);
}
__attribute__((interrupt)) void FloatingPointHandler(struct Registers64 *r)
{
    System::panic("The CPU was floating... But somehow, your code managed to get a gun and destroy the thing who makes the CPU float...", r);
}
__attribute__((interrupt)) void AlignmentCheckHandler(struct Registers64 *r)
{
    System::panic("Alminteng Halnred", r);
}
__attribute__((interrupt)) void MachineCheckHandler(struct Registers64 *r)
{
    System::panic("Machine check.", r);
}
__attribute__((interrupt)) void SimdFloatingPointHandler(struct Registers64 *r)
{
    System::panic("Simd Floating Point.", r);
}
__attribute__((interrupt)) void VirtualizationExceptionHandler(struct Registers64 *r)
{
    System::panic("Did you try to run KVM in SipaaKernel??", r);
}

__attribute__((interrupt)) void MouseHandler(struct Registers64 *r)
{
    Logger::Log(LogType_Debug, "Interrupt raised!\n");
    uint8_t mouseData = Io::InB(0x60);

    Dev::PS2::MouseHandler(mouseData);

    Idt::PicEoi(12);
}

__attribute__((interrupt)) void KeyboardHandler(struct Registers64 *r)
{
    static const char scancode_ascii[128] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
        '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    uint8_t scancode = Io::InB(0x60);
    char glyph = ' ';

    if (scancode & 0x80) {
        Idt::PicEoi(1); return;
    }

    if (scancode != 0x0E)
        if (scancode != 0x1C)
            glyph = scancode_ascii[scancode];

    if (glyph)
    {
        Sk::Dev::Serial::WriteChar(glyph);
    }

    Idt::PicEoi(1);
}

void Idt::PicEoi(uint8_t irq)
{
    if (irq >= 8)
        Io::OutB(PIC2_COMMAND, PIC_EOI);

    Io::OutB(PIC1_COMMAND, PIC_EOI);
}

void Idt::PicRemap()
{
    uint8_t a1, a2;

    a1 = Io::InB(PIC1_DATA);
    Io::Wait();
    a2 = Io::InB(PIC2_DATA);
    Io::Wait();

    Io::OutB(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    Io::Wait();
    Io::OutB(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    Io::Wait();

    Io::OutB(PIC1_DATA, 0x20);
    Io::Wait();
    Io::OutB(PIC2_DATA, 0x28);
    Io::Wait();

    Io::OutB(PIC1_DATA, 4);
    Io::Wait();
    Io::OutB(PIC2_DATA, 2);
    Io::Wait();

    Io::OutB(PIC1_DATA, ICW4_8086);
    Io::Wait();
    Io::OutB(PIC2_DATA, ICW4_8086);
    Io::Wait();

    Io::OutB(PIC1_DATA, a1);
    Io::Wait();
    Io::OutB(PIC2_DATA, a2);
}

void Idt::SetEntry(uint8_t vector, void *isr, uint8_t flags)
{
    IdtEntry *descriptor = &entries[vector];

    descriptor->base_low = (uint64_t)isr & 0xFFFF;
    descriptor->cs = 0x08;
    descriptor->ist = 0;
    descriptor->attributes = flags;
    descriptor->base_mid = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->base_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->rsv0 = 0;
}

void Idt::Init()
{
    Logger::Log(LogType_Info, "Starting IDT initialization...\n");

    Logger::Log(LogType_Debug, "Loading IDT pointer...");
    idtr.base = (uintptr_t)&entries[0];
    idtr.limit = (uint16_t)sizeof(IdtEntry) * 256 - 1;
    Logger::PrintOK();

    Logger::Log(LogType_Debug, "Loading IDT entries...");
    SetEntry(0, (uint64_t)DivZeroHandler, 0x8E);
    SetEntry(1, (uint64_t)DebugHandler, 0x8E);
    SetEntry(2, (uint64_t)NonMaskableInterruptHandler, 0x8E);
    SetEntry(3, (uint64_t)BreakPointHandler, 0x8E);
    SetEntry(4, (uint64_t)OverflowHandler, 0x8E);
    SetEntry(5, (uint64_t)BoundRangeExceeded, 0x8E);
    SetEntry(6, (uint64_t)InvalidOpcodeHandler, 0x8E);
    SetEntry(7, (uint64_t)DeviceNotAvailableHandler, 0x8E);
    SetEntry(8, (uint64_t)DoubleFaultHandler, 0x8E);
    SetEntry(10, (uint64_t)InvalidTssHandler, 0x8E);
    SetEntry(11, (uint64_t)SegNotPresentHandler, 0x8E);
    SetEntry(12, (uint64_t)StackFaultHandler, 0x8E);
    SetEntry(13, (uint64_t)GpfHandler, 0x8E);
    SetEntry(14, (uint64_t)PfHandler, 0x8E);
    SetEntry(16, (uint64_t)FloatingPointHandler, 0x8E);
    SetEntry(17, (uint64_t)AlignmentCheckHandler, 0x8E);
    SetEntry(18, (uint64_t)MachineCheckHandler, 0x8E);
    SetEntry(19, (uint64_t)SimdFloatingPointHandler, 0x8E);
    SetEntry(20, (uint64_t)VirtualizationExceptionHandler, 0x8E);

    SetEntry(33, (uint64_t)KeyboardHandler, 0x8E);
    SetEntry(44, (uint64_t)MouseHandler, 0x8E);

    Logger::PrintOK();

    Logger::Log(LogType_Debug, "Setting up PIC...");
    PicRemap();
    Io::OutB(PIC1_DATA, 0b11111101);
    Io::OutB(PIC2_DATA, 0b11111111);
    Logger::PrintOK();

    Logger::Log(LogType_Debug, "Loading IDT into CPU...");
    __asm__ volatile("lidt %0"
                     :
                     : "m"(idtr));
    Logger::PrintOK();

    Logger::Log(LogType_Debug, "Enabling interrupts...");
    __asm__ volatile("sti");
    Logger::PrintOK();

    Logger::Log(LogType_Info, "IDT initialization is sucessful!\n");
}

}
}