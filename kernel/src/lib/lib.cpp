#include "lib.h"

namespace Sk {

void Lib::IToString(int value, char *str, int base)
{
    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char *wstr = str;
    int sign;

    if (base < 2 || base > 35)
    {
        *wstr = '\0';
        return;
    }

    if ((sign = value) < 0)
        value = -value;

    do
        *wstr++ = num[value % base];
    while (value /= base);
    if (sign < 0)
        *wstr++ = '-';
    *wstr = '\0';

    ReverseString(str, wstr - 1);
}

void Lib::UToString(unsigned int value, char *str, int base)
{
    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char *wstr = str;

    if (base < 2 || base > 35)
    {
        *wstr = '\0';
        return;
    }

    do
        *wstr++ = num[value % base];
    while (value /= base);
    *wstr = '\0';

    ReverseString(str, wstr - 1);
}

void Lib::UlliToString(unsigned long long int value, char *str, int base)
{
    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char *wstr = str;

    if (base < 2 || base > 35)
    {
        *wstr = '\0';
        return;
    }

    do
        *wstr++ = num[value % base];
    while (value /= base);
    *wstr = '\0';

    ReverseString(str, wstr - 1);
}

void Lib::ReverseString(char *begin, char *end)
{
    char aux;
    while (end > begin)
        aux = *end, *end-- = *begin, *begin++ = aux;
}

char *Lib::CopyString(char *dest, const char *src)
{
    char *originalDest = dest;

    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = '\0';
    return originalDest;
}

}