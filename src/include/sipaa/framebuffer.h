/// @file src/include/sipaa/framebuffer.h
/// @brief Interface to control graphic adapters - Header

#ifndef __FRAMEBUFFER_H
#define __FRAMEBUFFER_H

#include <stdint.h>
#include <stdbool.h>

enum FramebufferBpp
{
    BPP32 = 0x20,
    BPP24 = 0x18,
    BPP16 = 0x10,
    BPP15 = 0x0F,
    BPP8 = 0x08,
    BPP4 = 0x04
};
typedef enum FramebufferBpp FramebufferBppT;

struct FramebufferMode
{
    uint32_t Width, Height, Pitch;
    FramebufferBppT Bpp;
};
typedef struct FramebufferMode FramebufferModeT;

struct Framebuffer
{
    uint64_t Address;
    int Size;

    FramebufferModeT Mode;

    uint8_t RedMaskShift;
    uint8_t RedMaskSize;
    uint8_t GreenMaskShift;
    uint8_t GreenMaskSize;
    uint8_t BlueMaskShift;
    uint8_t BlueMaskSize;
};
typedef struct Framebuffer FramebufferT;

void Fbuf_Initialize();

/// @brief Resize the framebuffer, if supported.
/// @param width The new width. Must be a multiple of 8 and less than 2560.
/// @param height The new height. Must be less than 1600.
/// @param bpp The BPP (bytes per pixel)
/// @param clearScreen If set to true, clear the screen before setting the mode.
/// @remark Framebuffer mode change is only supported with BGA (Bochs Graphics Adapter)
void Fbuf_SetMode(uint32_t width, uint32_t height, FramebufferBppT bpp, bool clearScreen);

/// @brief Get the max mode supported.
/// @return A framebuffer mode.
/// @remark If the max mode is unknown, the returned mode will be 0x0x4 (with pitch: 0)
FramebufferModeT Fbuf_GetMaxMode();

/// @brief Get the current framebuffer mode.
/// @return A framebuffer mode.
FramebufferModeT Fbuf_GetCurrentMode();

/// @brief Enable/disable the framebuffer. Only supported with BGA.
/// @param enabled True -> enabled, False -> disabled.
void Fbuf_SetEnabled(bool enabled);

#endif