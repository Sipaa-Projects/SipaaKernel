#include "conio.h"
#include <flanterm/backend-fb.h>
#include <logger/logger.h>

struct flanterm_context *ftctx;

/// @brief Write to the console
/// @param buf The buffer to write (exemple: "Hello World")
/// @param charsize The character size (usually 1 for ASCII)
/// @param len The string length (exemple: 11 for the exemple buffer)
void conio_write(char *buf, size_t charsize, size_t len)
{
    flanterm_write(ftctx, buf, (len * charsize));
}

/// @brief Change the console's background color
/// @param fg The new background color
void conio_chbg(uint32_t bg)
{
    ftctx->set_text_fg_rgb(ftctx, bg);
    //ftctx->set_text_fg(ftctx, fg);
}

/// @brief Change the console's foreground color
/// @param fg The new foreground color
void conio_chfg(uint32_t fg)
{
    //ftctx->set_text_bg(ftctx, bg);
    ftctx->set_text_fg_rgb(ftctx, fg);
}

/// @brief Reset the console's colors
void conio_rstcol()
{
    ftctx->set_text_bg_default(ftctx);
    ftctx->set_text_fg_default(ftctx);
}

/// @brief Initialize the ConIO driver
/// @param skgbi An SKGBI instance
void conio_initialize(sk_general_boot_info skgbi)
{
    ftctx = flanterm_fb_simple_init(skgbi.fb0.address, skgbi.fb0.width, skgbi.fb0.height, skgbi.fb0.pitch, NULL);
}