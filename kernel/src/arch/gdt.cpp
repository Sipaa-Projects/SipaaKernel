#ifndef __aarch64__
#include "gdt.h"

#include <memory/memory.h>
#include <logging/logger.h>

using namespace Sk::Memory;
using namespace Sk::Logging;

namespace Sk {
namespace Arch {

GdtEntry Gdt::entries[0xFF];
GdtPtr Gdt::ptr;
TssEntry Gdt::tssentry;

extern "C" void load_tss(void);
extern "C" void load_gdt(GdtPtr *);

void Gdt::SetKernelStack(uint64_t stack)
{
    tssentry.rsp0 = stack;
}

void Gdt::InitTss(uint64_t kernel_stack)
{
    BasicMemoryManagement::MemorySet32(&tssentry, 0, sizeof(TssEntry));
    tssentry.rsp0 = kernel_stack;
}

void Gdt::SetEntry(int i, uint16_t limit, uint16_t base, uint8_t access, uint8_t flags)
{
    entries[i].base0 = base & 0xFFFF;
    entries[i].base1 = (base >> 16) & 0xFF;
    entries[i].base2 = (base >> 24) & 0xFF;
    entries[i].limit0 = limit & 0xFFFF;
    entries[i].access = access;
    entries[i].limit1_flags = ((limit >> 16) & 0x0F) | (flags & 0xF0);
}

void Gdt::SetTssGate(int32_t num, uint64_t base, uint32_t limit)
{
    entries[num].base0 = base & 0xFFFF;
    entries[num].base1 = (base >> 16) & 0xFF;
    entries[num].base2 = (base >> 24) & 0xFF;
    entries[num].limit0 = limit & 0xFFFF;
    entries[num].access = 0xE9;
    entries[num].limit1_flags = ((limit >> 16) & 0x0F) | 0x00;
}

int Gdt::Init(uint64_t kernel_stack)
{
    Logger::Log(LogType_Info, "GDT", "Starting initialization...\n");

    Logger::Log(LogType_Debug, "GDT", "Initializing pointer...");
    ptr.size = sizeof(entries) - 1;
    ptr.offset = (uint64_t)&entries;
    Logger::PrintOK();

    Logger::Log(LogType_Debug, "GDT", "Initializing entries...");
    SetEntry(0, 0, 0, 0, 0);
    SetEntry(1, 0, 0, 0x9A, 0xA0);
    SetEntry(2, 0, 0, 0x92, 0xA0);
    SetEntry(3, 0, 0, 0xFA, 0xA0);
    SetEntry(4, 0, 0, 0xF2, 0xA0);
    Logger::PrintOK();

    Logger::Log(LogType_Debug, "GDT", "Initializing TSS...");
    InitTss(kernel_stack);
    Logger::PrintOK();
    
    Logger::Log(LogType_Debug, "GDT", "Setting up TSS gate in GDT...");
    SetTssGate(5, (uint64_t)&tssentry, sizeof(TssEntry));
    Logger::PrintOK();

    Logger::Log(LogType_Debug, "GDT", "Loading into CPU...");
    load_gdt(&ptr);
    Logger::PrintOK();

    Logger::Log(LogType_Debug, "GDT", "Loading TSS into CPU...");
    load_tss();
    Logger::PrintOK();

    Logger::Log(LogType_Info, "GDT", "Initialization is sucessful!\n");
    return 0;
}

} // namespace Arch
}
#endif