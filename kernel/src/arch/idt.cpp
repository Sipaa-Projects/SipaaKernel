#include "idt.h"

#include "io.h"

#include <global.h>
#include <dev/serial.h>
#include <console/console.h>
#include <logging/logger.h>

using namespace Sk::Logging;

namespace Sk {
namespace Arch {

IdtEntry Idt::entries[256];
IdtPtr Idt::idtr;

__attribute__((interrupt)) void BreakPointHandler(struct Registers64 *r)
{
    Global::Framebuffer.UseDoubleBuffer = false;
    Console::Reset();

    Sk::Logging::Logger::Log(Sk::Logging::LogType::LogType_Error, "Kernel Panic :'( (BreakPoint)");

    while (1)
        asm("hlt");
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
    SetEntry(3, (uint64_t)BreakPointHandler, 0x8E);
    SetEntry(33, (uint64_t)KeyboardHandler, 0x8E);
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