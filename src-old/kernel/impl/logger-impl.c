#include "logger-impl.h"
#include <device/serial/serial.h>
#include <logger/logger.h>
#include <entry/limine.h>
#include <device/conio/conio.h>

/// @brief The line colors for the serial console.
char *lineColors[LT_LENGTH] = {
    "\033[36m",
    "\033[33m",
    "\033[31m"
};

/// @brief The line colors for the graphical console.
uint32_t graphicalLineColors[LT_LENGTH] = {
    0x03b6fc,
    0xfcc203,
    0xff0000
};

/// @brief SK implementation for the logger.
/// @param lt Log type
/// @param isLtText Does the text to print is the log type text?
/// @param text The text to print
/// @param size The text size
void logger_write_sk_impl(enum LogType lt, int isLtText, char *text, int size)
{
    if (isLtText == 1 && lt != LT_LENGTH)
    {
        #if defined(__x86_64__) | defined(__i686__) | defined(__i386__)
        serial_puts(lineColors[lt]);
        #endif
        #if defined(SKC_LOGSCONIO)
        conio_chfg(graphicalLineColors[lt]);
        //logger_ftctx->set_text_fg_rgb(logger_ftctx, graphicalLineColors[lt]);
        #endif
    }

    for (int i = 0; i < size; i++)
    {
        #if defined(__x86_64__) | defined(__i686__) | defined(__i386__)
        serial_putc(text[i]);
        #endif
    }
    #if defined(SKC_LOGSCONIO)
    conio_write(text, sizeof(char), size);
    #endif

    if (isLtText == 1)
    {
        #if defined(__x86_64__) | defined(__i686__) | defined(__i386__)
        serial_puts("\033[0m");
        #endif
        #if defined(SKC_LOGSCONIO)
        conio_rstcol();
        //logger_ftctx->set_text_fg_default(logger_ftctx);
        #endif
    }
}

/// @brief Initialize the SK implementation for the logger
void logger_sk_impl_init() {
    logger_write = &logger_write_sk_impl;
}