// TODO: use uart to print to serial console.

#include <sk-hal/serial.h>

/// @brief Initialize serial (aarch64 specific) (not implemented)
void serial_init() { }

/// @brief Print a character to the serial console (aarch64 specific) (not implemented)
/// @param a The character to print
void serial_putc(char a) { (void)a; }

/// @brief Print a string to the serial console (aarch64 specific) (not implemented)
/// @param s The string to print
void serial_puts(const char *s) { (void)s; }