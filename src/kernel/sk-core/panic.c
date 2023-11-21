#include "panic.h"
#include <sk-hal/hal.h>
#include <sk-logger/logger.h>

/// @brief Internal kernel panic function. Is isn't recommmended to call directly this function. Use the 'panic' macro instead
/// @param file The file
/// @param line The line
/// @param message The message
void __sk_core_internal_panic(char *file, char *line, char *message, ...)
{
    log_nofileinfo(LT_ERROR, "Kernel Panic\n");
    log_nofileinfo(LT_ERROR, "panic() called at %s, line %s\n", file, line);
    va_list args;
    va_start(args, message);
    log_nofileinfo(LT_ERROR, message, args);
    va_end(args);
    log_nofileinfo(LT_ERROR, "We are sorry for the inconvenience.\n");
    log_nofileinfo(LT_ERROR, "System halted.\n");
    while (1)
        ;;
    //hal_cpu_stop();
}