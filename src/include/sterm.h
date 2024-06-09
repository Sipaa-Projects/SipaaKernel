#pragma once

#include <stdint.h>
#include <stddef.h>

#define PSF1_FONT_MAGIC 0x0436
 
typedef struct {
    uint16_t magic; // Magic bytes for identification.
    uint8_t fontMode; // PSF font mode.
    uint8_t characterSize; // PSF character size.
} psf1_font_t;

struct terminal
{
    uint32_t *framebuffer_address;
    int width;
    int height;
    int cursor_x;
    int cursor_y;
    uint32_t defbg;
    uint32_t deffg;
    uint32_t bg;
    uint32_t fg;
    psf1_font_t *psf1font;
    uint16_t *unicode;
    void *font;
    int font_width;
    int font_height;
    int font_spacing;
};

void sterm_init(
    uint32_t *fbptr, int fbwidth, int fbheight,
    uint32_t defbg, uint32_t deffg,
    void *font, int fontwidth, int fontheight, int fontspacing);

void sterm_clear();
void sterm_set_bg(uint32_t bgc);
void sterm_set_fg(uint32_t fgc);
void sterm_put_string(const char *string);