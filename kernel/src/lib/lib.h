#pragma once

#include <cstdint>

namespace Sk {

class Lib {
public:
    static int StringCompare(const char *str1, const char *str2);
    static int ToUpper(int c);
    static int ToLower(int c);
    static void IToString(int value, char *str, int base);
    static void UToString(unsigned int value, char *str, int base);
    static void UlliToString(unsigned long long int value, char *str, int base);
    static void ReverseString(char *begin, char *end);
    static char *CopyString(char *dest, const char *src);
};

}