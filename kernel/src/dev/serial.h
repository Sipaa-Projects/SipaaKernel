#pragma once

#include <stdint.h>
#include <stdarg.h>

#define SERIAL_COLOR_BLUE "\033[1;32m"
#define SERIAL_COLOR_GREEN "\033[1;34m"
#define SERIAL_COLOR_YELLOW "\033[1;33m"
#define SERIAL_COLOR_RED "\033[0;31m"
#define SERIAL_COLOR_WHITE "\033[0;37m"

#define SERIAL_PORT 0x3f8

namespace Sk {
namespace Dev {

class Serial {
public:
    static void Init();
    static bool IsReady();
    static void SetColor(char *color);
    static void WriteStr(char *string);
    static void WriteChar(char ch);
    static void WriteInt(int v, int base, const char *digits);
    static void WriteInt64(uint64_t v, int base, const char *digits);
    static void WriteStrFormatted(char *str, ...);
};

} // namespace Dev
} // namespace Sk