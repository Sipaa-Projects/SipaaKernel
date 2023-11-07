#ifndef __PANIC_H
#define __PANIC_H

#include <stdarg.h>

void __sk_core_internal_panic(char *file, char *line, char *message, ...);

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define panic(message, ...) __sk_core_internal_panic(__FILE__, STRINGIZE(__LINE__), message, ##__VA_ARGS__);

#endif