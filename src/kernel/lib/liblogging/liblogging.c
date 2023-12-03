#include "liblogging.h"

#include <stdarg.h>
#include <video/video.h>

void log(int level, const char *message, ...)
{
    switch (level)
    {
    case LOG_ERROR:
        set_color(0xC0392B);
        if (!usedoublebuffering)
            printf("[ERROR] ");
        serial_puts("\033[0;31m[ERROR] \033[0;37m");
        break;
    case LOG_WARN:
        set_color(0xF39C12);
        if (!usedoublebuffering)
            printf("[WARN] ");
        serial_puts("\033[1;33m[WARN] \033[0;37m");
        break;
    case LOG_INFO:
        set_color(0x27AE60);
        if (!usedoublebuffering)
            printf("[INFO] ");
        serial_puts("\033[1;32m[INFO] \033[0;37m");
        break;
    case LOG_DEBUG:
        set_color(0x2980B9);
        if (!usedoublebuffering)
            printf("[DEBUG] ");
        serial_puts("\033[1;34m[DEBUG] \033[0;37m");
        break;
    }

    set_color(0xFFFFFF);

    va_list args;
    va_start(args, message);

    if (!usedoublebuffering)
        printf(message, args);

    serial_putsf(message, args);
    va_end(args);
}