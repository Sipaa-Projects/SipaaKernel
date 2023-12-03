#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

char scancode_to_ascii(uint8_t scancode);
uint32_t random();
bool is_rdseed_available();
void syscall(int rax, void *rbx, void *rcx, void *rdx, void *rsi, void *rdi);
void outb(unsigned short port, unsigned char value);
unsigned char inb(unsigned short port);
void insl(uint16_t port, void *addr, int cnt);
void outw(unsigned short port, unsigned short value);
void outsw(uint16_t port, const void *addr, uint32_t count);
unsigned short inw(unsigned short port);
void insw(uint16_t port, void *addr, int cnt);
void io_wait();
void *memcpy(void *dest, void *src, size_t count);
void *memset(void *ptr, int value, size_t count);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
size_t strlen(const char *str);
int strcmp(const char *str1, const char *str2);
char *strcpy(char *dest, const char *src);
char *strncpy(char *destination, const char *source, size_t num);
int strncmp(const char *s1, const char *s2, size_t n);
void strreverse(char *begin, char *end);
int toupper(int c);
int tolower(int c);
void itoa(int value, char *str, int base);
void utoa(unsigned int value, char *str, int base);
void ullitoa(unsigned long long int value, char *str, int base);
void delay(uint16_t ms);

#endif