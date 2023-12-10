#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Thanks chatgpt for this file :sweat_smile:

char scancode_to_ascii(uint8_t scancode)
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

    return scancode_ascii[scancode];
}

#ifdef __x86_64__
bool is_rdseed_available()
{
    uint32_t ecx_flag = 0x0;
    uint32_t bit_rdseed = 0x18;  // Bit 18 of ECX register indicates 'rdseed' support

    // Execute CPUID with EAX=7 to get feature information (EAX=7, ECX=0)
    __asm__ volatile (
        "cpuid"
        : "=c" (ecx_flag)   // Output: ECX register
        : "a" (7), "c" (0)  // Input: EAX=7, ECX=0
    );

    // Check if 'rdseed' bit (bit 18) is set in ECX
    return (ecx_flag & (1 << bit_rdseed)) != 0;
}

uint32_t random() {
    if (!is_rdseed_available()) {
        return 0;  // 'rdseed' instruction not available
    }

    uint32_t random_value;
    bool success;

    // Use 'rdseed' instruction to obtain random values
    __asm__ volatile (
       "rdseed %0"
        : "=r" (random_value), "=a" (success)
    );

    if (!success) {
        return false;  // Error occurred while obtaining random value
    }

    return random_value;  // Random numbers generated successfully
}

void syscall(int rax, void *rbx, void *rcx, void *rdx, void *rsi, void *rdi)
{
    __asm__ volatile(
        "mov %1, %%rbx\n"
        "mov %2, %%rcx\n"
        "mov %3, %%rdx\n"
        "mov %4, %%rsi\n"
        "mov %5, %%rdi\n"
        "int $0x80\n"
        :
        : "r" (rax), "r" (rbx), "r" (rcx), "r" (rdx), "r" (rsi), "r" (rdi)
        : "%rbx", "%rcx", "%rdx", "%rsi", "%rdi"
    );
}
#endif

void *memcpy(void* dest, void* src, size_t count) {
    char *dest_c = (char *)dest;
    const char *src_c = (const char *)src;
    for (size_t i = 0; i < count; i++)
    {
        dest_c[i] = src_c[i];
    }
    return dest;
}

void *memset(void *ptr, int value, size_t count)
{
    char *ptr_c = (char *)ptr;
    for (size_t i = 0; i < count; i++)
    {
        ptr_c[i] = (char)value;
    }
    return ptr;
}

void *memmove(void *dest, const void *src, size_t n)
{
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    if (s < d && (s + n) > d)
    {
        s += n;
        d += n;
        while (n-- > 0)
            *--d = *--s;
    }
    else
    {
        while (n-- > 0)
            *d++ = *s++;
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    const unsigned char *p1 = s1, *p2 = s2;
    while (n--)
    {
        if (*p1 != *p2)
        {
            return *p1 - *p2;
        }
        p1++;
        p2++;
    }
    return 0;
}

int strcmp(const char *str1, const char *str2)
{
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }

    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

char *strchr(const char *str, int character)
{
    while (*str != '\0')
    {
        if (*str == character)
            return (char *)str;
        str++;
    }
    return NULL;
}

size_t strlen(const char *str)
{
    const char *s = str;
    while (*s)
        s++;
    return s - str;
}

char *strcpy(char *dest, const char *src)
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

char *strncpy(char *destination, const char *source, size_t num)
{
    char *ptr = destination;
    size_t i = 0;

    while (*source && i < num)
    {
        *ptr++ = *source++;
        i++;
    }

    while (i < num)
    {
        *ptr++ = '\0';
        i++;
    }

    return destination;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (s1[i] != s2[i])
        {
            return (unsigned char)s1[i] - (unsigned char)s2[i];
        }
        else if (s1[i] == '\0')
        {
            return 0;
        }
    }
    return 0;
}

void strreverse(char *begin, char *end)
{
    char aux;
    while (end > begin)
        aux = *end, *end-- = *begin, *begin++ = aux;
}

int toupper(int c)
{
    if (c >= 'a' && c <= 'z')
    {
        return c - ('a' - 'A');
    }
    else
    {
        return c;
    }
}

int tolower(int c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c + ('a' - 'A');
    }
    else
    {
        return c;
    }
}

void itoa(int value, char *str, int base)
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

    strreverse(str, wstr - 1);
}

void utoa(unsigned int value, char *str, int base)
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

    strreverse(str, wstr - 1);
}

void ullitoa(unsigned long long int value, char *str, int base)
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

    strreverse(str, wstr - 1);
}

#ifdef __x86_64__
void delay(uint16_t ms)
{
    for (long long int i = 0; i < 5000 * (uint16_t)ms / 2; i++)
        io_wait();
}
#endif