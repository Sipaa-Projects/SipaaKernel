#pragma once

#include <cstdint>

namespace Sk {
namespace Arch {

class Io {
public:
    static void OutB(uint16_t port, uint8_t value);
    static void OutW(uint16_t port, uint16_t value);
    static void OutSW(uint16_t port, const void *addr, uint32_t count);
    static void OutL(uint16_t port, uint32_t value);

    static uint8_t InB(uint16_t port);
    static uint16_t InW(uint16_t port);
    static void InSW(uint16_t port, void *addr, int cnt);
    static uint32_t InL(uint16_t port);

    static void Wait();
};

} // namespace Arch
} // namespace Sk