#pragma once

#include <cstdint>

namespace Sk {

class Lib {
public:
    static void IToString(int value, char *str, int base);
    static void UToString(unsigned int value, char *str, int base);
    static void UlliToString(unsigned long long int value, char *str, int base);
    static void ReverseString(char *begin, char *end);
    static char *CopyString(char *dest, const char *src);
};

}