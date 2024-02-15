/// @brief ConIO is a driver which manages console input/output.

#include "conio.h"
#include <flanterm/backend-fb.h>
#include <logger/logger.h>
#include <memory/bmo.h>
#include <memory/pmm.h>
#include <fs/devfs/devfs.h>

struct flanterm_context *ftctx;

/// @brief Write to the console
/// @param buf The buffer to write (exemple: "Hello World")
/// @param charsize The character size (usually 1 for ASCII)
/// @param len The string length (exemple: 11 for the exemple buffer)
void conio_write(char *buf, size_t charsize, size_t len)
{
    if (!ftctx)
        return;
    flanterm_write(ftctx, buf, (len * charsize));
}

/// @brief Change the console's background color
/// @param fg The new background color
void conio_chbg(uint32_t bg)
{
    if (!ftctx)
        return;
    ftctx->set_text_fg_rgb(ftctx, bg);
    //ftctx->set_text_fg(ftctx, fg);
}

/// @brief Change the console's foreground color
/// @param fg The new foreground color
void conio_chfg(uint32_t fg)
{
    if (!ftctx)
        return;
    //ftctx->set_text_bg(ftctx, bg);
    ftctx->set_text_fg_rgb(ftctx, fg);
}

/// @brief Reset the console's colors
void conio_rstcol()
{
    if (!ftctx)
        return;
    ftctx->set_text_bg_default(ftctx);
    ftctx->set_text_fg_default(ftctx);
}

/// @brief Clear the console
void conio_clear()
{
    ftctx->clear(ftctx, true);
    //ftctx->set_cursor_pos(ftctx, 0, 0);
}

char* strcpy_custom(char *dest, const char *src) {
    // Check for null pointers
    if (dest == NULL || src == NULL) {
        return NULL;
    }

    char *originalDest = dest;

    // Copy each character from src to dest
    while ((*dest++ = *src++) != '\0');

    return originalDest;
}

/// @brief Initialize the ConIO driver
void conio_initialize()
{
    log(LT_INFO, "conio", "initializing...\n");
    // TODO: bring the framebuffer to it's own file
    struct limine_framebuffer *fb = entry_getfb();
    ftctx = flanterm_fb_simple_init(fb->address, fb->width, fb->height, fb->pitch, NULL);

    FILE *conio_file = (FILE *)pmm_alloc(sizeof(FILE));

    strcpy_custom(conio_file->name, "conio");
    conio_file->write = conio_write;

    devfs_add_device(conio_file);
    log(LT_INFO, "conio", "initialized.\n");
}