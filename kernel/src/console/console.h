#pragma once

#include <cstdint>
#include <cstdarg>
#include <video/video.h>
using namespace Sk::Graphic;

namespace Sk {

typedef struct
{
    uint8_t magic[2];
    uint8_t mode;
    uint8_t height;
} PsfFontHeader;

class Console {

public:
    static uint32_t term_color;
    static int termx, termy;
    static Framebuffer framebuf;

    static void Init(Framebuffer f);
    static void ScrollUp();
    static void Reset();
    static void PrintChar(char c);
    static void Print(char *str);
    static void PrintFormatted(char *str, ...);
};
} // namespace Sk