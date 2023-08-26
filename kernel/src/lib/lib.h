#pragma once

#include <stdint.h>

namespace Sk {

class Lib {
public:
    static uint32_t RNGSeed;

    static void InitRNG(uint32_t Seed);
    static uint32_t Random();
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