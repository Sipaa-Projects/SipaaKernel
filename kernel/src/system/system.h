#pragma once

#include <cstdint>
#include <arch/idt.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

using namespace Sk::Arch;

namespace Sk {

class System {
private:
    static void _panic(char *msg, char *file, char *line, struct Registers64 *regs);
public:
    static void panic(char *msg, struct Registers64 *regs)
    {
        _panic(msg, __FILE__, STRINGIFY(__LINE__), regs);
    }
};

}