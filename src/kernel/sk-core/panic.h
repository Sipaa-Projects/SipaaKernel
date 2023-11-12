#ifndef __PANIC_H
#define __PANIC_H

#include <stdarg.h>

/// @brief Internal kernel panic function. Is isn't recommmended to call directly this function. Use the 'panic' macro instead
/// @param file The file
/// @param line The line
/// @param message The message
void __sk_core_internal_panic(char *file, char *line, char *message, ...);

/// @brief Convert an integer macro to a string
#define STRINGIZE(x) STRINGIZE2(x)

/// @brief Second function to convet integer macros to string. Needed because of macro behavior. Use 'STRINGIZE' macro instead
#define STRINGIZE2(x) #x

/// @brief Do a Kernel Panic error screen.
/// @param message The message
#define panic(message, ...) __sk_core_internal_panic(__FILE__, STRINGIZE(__LINE__), message, ##__VA_ARGS__);

#endif