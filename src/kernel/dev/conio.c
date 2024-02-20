#include <sipaa/conio.h>
#include <flanterm/backends/fb.h>
#include <flanterm/flanterm.h>
#include <stdarg.h>
#include <nanoprintf/printf.h>

#define cvtcol(CTX, COLOUR) (COLOUR)
struct flanterm_fb_context *ftctx;

int __conio_internals_strlen(char *str);

void conio_setbgframebuf(uint32_t *buffer)
{
    for (size_t i = 0; i < ftctx->width * ftctx->height; i++)
    {
        ftctx->canvas[i] = cvtcol(ftctx, buffer[i]);
    }
}

/// @brief Clear the console
void conio_clear()
{
    ftctx->term.clear(ftctx, true);
}

/// @brief Print to the console
/// @param buf The buffer to write (exemple: "Hello World")
void conio_print(char *buf)
{
    flanterm_write(&ftctx->term, buf, __conio_internals_strlen(buf));
}

/// @brief Print to the console, and add a new line character to the end
/// @param buf The buffer to write (exemple: "Hello World")
void conio_println(char *buf)
{
    conio_print(buf);
    conio_print("\n");
}

void conio_printf(char *format, ...)
{
    va_list args;
    va_start(args, format);

    char buf[32767];

    npf_vsnprintf(buf, sizeof(buf), format, args); // Use nanoprintf for formatting.

    conio_print(&buf);

    va_end(args);
}

void conio_vprintf(char *format, va_list args)
{
    char buf[32767];

    npf_vsnprintf(buf, sizeof(buf), format, args); // Use nanoprintf for formatting.

    conio_print(buf);
}

void conio_printfln(char *format, ...)
{
    va_list args;
    va_start(args, format);

    char buf[32767];

    npf_vsnprintf(buf, sizeof(buf), format, args); // Use nanoprintf for formatting.

    conio_print(buf);

    va_end(args);

    conio_print("\n");
}

/// @brief Change the console's background color
/// @param fg The new background color
void conio_chbg(uint32_t bg)
{
    ftctx->term.set_text_bg_rgb(ftctx, bg);
    // ftctx->set_text_fg(ftctx, fg);
}

/// @brief Change the console's foreground color
/// @param fg The new foreground color
void conio_chfg(uint32_t fg)
{
    // ftctx->set_text_bg(ftctx, bg);
    ftctx->term.set_text_fg_rgb(ftctx, fg);
}

/// @brief Reset the console's colors
void conio_rstcol()
{
    ftctx->term.set_text_bg_default(ftctx);
    ftctx->term.set_text_fg_default(ftctx);
}

int __conio_internals_strlen(char *str)
{
    int size = 0;

    while (*str != '\0')
    {
        size++;
        str++;
    }

    return size;
}

/// @brief Initialize the ConIO driver
/// @param skgbi An SKGBI instance
void conio_initialize(sk_general_boot_info skgbi)
{
    size_t font_scale_x = 1;
    size_t font_scale_y = 1;
    if (skgbi.fb0.width >= (1920 + 1920 / 3) && skgbi.fb0.height >= (1080 + 1080 / 3)) {
        font_scale_x = 2;
        font_scale_y = 2;
    }
    if (skgbi.fb0.width >= (3840 + 3840 / 3) && skgbi.fb0.height >= (2160 + 2160 / 3)) {
        font_scale_x = 4;
        font_scale_y = 4;
    }

    ftctx = flanterm_fb_init(
        NULL, 
        NULL, 
        skgbi.fb0.address, 
        skgbi.fb0.width, 
        skgbi.fb0.height, 
        skgbi.fb0.pitch, 
        NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, 0, 0, 1,
        font_scale_x, font_scale_y,
        0);
}