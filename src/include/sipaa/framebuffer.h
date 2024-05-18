/// @file src/include/sipaa/framebuffer.h
/// @brief Interface to control graphic adapters - Header

#ifndef __FRAMEBUFFER_H
#define __FRAMEBUFFER_H

#include <stdint.h>
#include <stdbool.h>

/// @brief All bytes-per-pixel values
enum FramebufferBpp
{
    /// @brief BPP24 but with alpha channel. More recommended over BPP24.
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

enum FramebufferGraphicsAcceleration
{
    /// @brief The Bochs graphics adapter
    BOCHSGA,

    /// @brief The VMware SVGA II graphics adapter
    VMSVGAII,

    /// @brief The Intel i915 driver
    I915,

    /// @brief No graphics acceleration - pure VBE/GOP.
    NONE
};
typedef enum FramebufferGraphicsAcceleration FramebufferGraphicsAccelerationT;

// Functions used in FramebufferCapabilitesT
typedef void(*Framebuffer_SetModeT)(FramebufferT *, FramebufferModeT);
typedef void(*Framebuffer_EnableT)(int);

/// @brief Defines the capabilities of the actual graphics adapter.
typedef struct FramebufferCapabilities
{
    FramebufferGraphicsAccelerationT FramebufferGraphicsAccelertion;

    bool CanSetModes;
    Framebuffer_SetModeT SetMode;
    bool CanBeDisabled;
    Framebuffer_EnableT Enable;

    FramebufferModeT MaxMode;
};
typedef struct FramebufferCapabilities FramebufferCapabilitiesT;

/// @brief Initialize the framebuffer
void Fbuf_Initialize();

/// @brief Resize the framebuffer, if supported.
/// @param mode The new mode
/// @remark Framebuffer mode change is only supported with BGA (Bochs Graphics Adapter) and VMware SVGA II
void Fbuf_SetMode(FramebufferModeT mode);

/// @brief Get the max mode supported.
/// @return A framebuffer mode.
/// @remark If the max mode is unknown, the returned mode will be 0x0x4 (with pitch: 0)
FramebufferModeT Fbuf_GetMaxMode();

/// @brief Get a pointer to the framebuffer.
/// @return A framebuffer.
FramebufferT *Fbuf_Get();

/// @brief Enable/disable the framebuffer. Only supported with BGA.
/// @param enabled True -> enabled, False -> disabled.
void Fbuf_SetEnabled(bool enabled);

#endif