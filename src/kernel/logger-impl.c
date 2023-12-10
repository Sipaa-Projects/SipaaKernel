#include "logger-impl.h"
#include <device/serial/serial.h>
#include <logger/logger.h>
#include <limine/limine.h>

/// @brief The flanterm context used by the logger to print logs to screen.
struct flanterm_context *logger_ftctx = NULL;

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

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
        #ifdef __x86_64__
        serial_puts(lineColors[lt]);
        #endif
        logger_ftctx->set_text_fg_rgb(logger_ftctx, graphicalLineColors[lt]);
    }

    for (int i = 0; i < size; i++)
    {
        #ifdef __x86_64__
        serial_putc(text[i]);
        #endif
        if (logger_ftctx)
            flanterm_write(logger_ftctx, &text[i], 1);
    }

    if (isLtText == 1)
    {
        #ifdef __x86_64__
        serial_puts("\033[0m");
        #endif
        logger_ftctx->set_text_fg_default(logger_ftctx);
    }
}

/// @brief Initialize the SK implementation for the logger
void logger_sk_impl_init() {
    #ifdef __x86_64__
    init_serial();
    #endif
    struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];

    logger_ftctx = flanterm_fb_simple_init(
        fb->address, fb->width, fb->height, fb->pitch, NULL
    );
    logger_write = &logger_write_sk_impl;
}