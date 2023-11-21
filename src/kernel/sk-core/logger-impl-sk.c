#include "bmo.h"
#include "pmm.h"
#include "panic.h"
#include <stdint.h>
#include <flanterm/flanterm.h>
#include <flanterm/backend-fb.h>
#include <serial.h>
#include <sk-logger/logger.h>
#include <entry.h>
#include <stdbool.h>

void *pmm_realloc(void *ptr, size_t newsize)
{
    pmm_free(ptr);
    return pmm_alloc(newsize);
}
void sk_assert(bool condition)
{
    if (condition == false)
        panic("Assertion failed.");
}
#define STBI_ASSERT(x) sk_assert(x)
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STBI_NO_SIMD
#define STBI_MALLOC(sz)           pmm_alloc(sz)
#define STBI_REALLOC(p,newsz)     pmm_realloc(p,newsz)
#define STBI_FREE(p)              pmm_free(p)
#define STBI_NO_STDIO 1 // Don't allow stbi to use stdio, to prevent conflicts
#define STB_IMAGE_IMPLEMENTATION
//#include <stb/stb_image.h>

struct limine_internal_module background = {
    .path = "/wallp.jpg"};

struct limine_internal_module *internal_modules[] = {
    &background};

struct limine_module_request module_request = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 1,
    .response = NULL,

    .internal_module_count = 1,
    .internal_modules = internal_modules};

/// @brief The flanterm context used by the logger to print logs to screen.
struct flanterm_context *logger_ftctx = NULL;

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
        serial_puts(lineColors[lt]);
        logger_ftctx->set_text_fg_rgb(logger_ftctx, graphicalLineColors[lt]);
    }

    for (int i = 0; i < size; i++)
    {
        serial_putc(text[i]);
        if (logger_ftctx)
            flanterm_write(logger_ftctx, &text[i], 1);
    }

    if (isLtText == 1)
    {
        serial_puts("\033[0m");
        logger_ftctx->set_text_fg_default(logger_ftctx);
    }
}

/// @brief Initialize the SK implementation for the logger
void logger_sk_impl_init() {
    struct limine_framebuffer *fb = limine_getfbufs()->framebuffers[0];

    //struct limine_file *wallp = module_request.response->modules[0];
    //int w = 0;
    //int h = 0;
    //int ch = 0;
    //uint32_t *wallpaper = (uint32_t *)stbi_load_from_memory((stbi_uc *)wallp->address, wallp->size, &w, &h, &ch, STBI_rgb_alpha);

    logger_ftctx = flanterm_fb_simple_init(
        fb->address, fb->width, fb->height, fb->pitch, NULL
    );
    logger_write = &logger_write_sk_impl;
}

/// @brief Initialize the SK implementation for the logger
/**void logger_sk_impl_init() {
    struct limine_framebuffer *fb = limine_getfbufs()->framebuffers[0];

    logger_ftctx = flanterm_fb_simple_init(
        fb->address, fb->width, fb->height, fb->pitch, image
    );
    logger_write = &logger_write_sk_impl;
}**/