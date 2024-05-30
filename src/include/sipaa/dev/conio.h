#pragma once

#include <sipaa/libc/types.h>

struct ConIO_RGBColor {
    I8 r;
    I8 g;
    I8 b;
};

typedef struct ConIO_RGBColor ConIO_RGBColorT;

/// @brief Clear the console
void ConIO_Clear();

/// @brief Print to the console
/// @param buf The buffer to write (exemple: "Hello World")
void ConIO_Print(char *buf);

/// @brief Change the console's background color
/// @param fg The new background color
void ConIO_SetBg(UI32 bg);

/// @brief Change the console's foreground color
/// @param fg The new foreground color
void ConIO_SetFg(UI32 fg);

/// @brief Reset the console's colors
void ConIO_ResetColor();

/// @brief Initialize the ConIO driver
void ConIO_Initialize();