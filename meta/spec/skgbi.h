/*
 * SKGBI - SipaaKernel General Boot Informations (Header version 1.1, Kept for posterity)
 *
 * MIT License
 *
 * Copyright (c) 2023 Sipaa Projects
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __SKGBI_H
#define __SKGBI_H

#include <stdint.h>

#define SKGBI_MEMMAP_USABLE                 0
#define SKGBI_MEMMAP_RESERVED               1
#define SKGBI_MEMMAP_ACPI_RECLAIMABLE       2
#define SKGBI_MEMMAP_ACPI_NVS               3
#define SKGBI_MEMMAP_BAD_MEMORY             4
#define SKGBI_MEMMAP_BOOTLOADER_RECLAIMABLE 5
#define SKGBI_MEMMAP_KERNEL_AND_MODULES     6
#define SKGBI_MEMMAP_FRAMEBUFFER            7
#define SKGBI_MEMMAP_MAX_ENTRIES            40

typedef struct {
    #if defined(__i686__) | defined(__i386__)
    void *base;
    uint32_t length;
    uint32_t type;
    #else
    uint64_t base;
    uint64_t length;
    uint64_t type;
    #endif
} sk_memory_mapentry;

typedef struct {
    sk_memory_mapentry entries[SKGBI_MEMMAP_MAX_ENTRIES];
    uint32_t length;
} sk_memory_info;

typedef struct {
    void* address;
    uint64_t pitch;
    uint64_t width;
    uint64_t height;
    uint16_t bpp;
    uint8_t memory_model;
    uint8_t red_mask_size;
    uint8_t red_mask_shift;
    uint8_t green_mask_size;
    uint8_t green_mask_shift;
    uint8_t blue_mask_size;
    uint8_t blue_mask_shift;
} sk_framebuffer;

typedef struct {
    sk_memory_info memory_map;
    sk_framebuffer fb0;
} sk_general_boot_info;

#endif