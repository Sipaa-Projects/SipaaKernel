#pragma once

#include <stdint.h>

/// @brief Initialize serial
void serial_init();

/// @brief Print a character to the serial console
/// @param a The character to print
void serial_putc(char a);

/// @brief Print a string to the serial console
/// @param s The string to print
void serial_puts(const char *s);