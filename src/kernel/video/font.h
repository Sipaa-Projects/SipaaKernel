#ifndef FONT_H
#define FONT_H

#include <stdint.h>

extern unsigned char font[];

typedef struct
{
    uint8_t magic[2];
    uint8_t mode;
    uint8_t height;
} font_header;

#endif