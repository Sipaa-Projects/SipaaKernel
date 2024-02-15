/*
 * MIT License
 *
 * Copyright (c) 2024 Sipaa Projects
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
 * 
 * This file is part of the Sipaa C Library.
 */

#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>

#define EOF (-1)

typedef struct FILE {
    int handle;                                                          // File descriptor or handle
    uint8_t attribs;                                                     // File attributes
    char *name;                                                          // File name
    char *path;                                                          // File's full path
    unsigned char *buffer;                                               // Data buffer (NULL if it's a directory)
    unsigned int buf_size;                                               // Size of the data buffer (0 if it's a directory)
    unsigned int buf_pos;                                                // Current position in the buffer (0 if it's a directory)
    int mode;                                                            // File access mode
    void (*read)(char* buffer, size_t size, size_t offset);
    void (*write)(const char* buffer, size_t size, size_t offset);       // Pointer to a function which writes to the file

    /// Directory-specific fields.
    struct FILE **children;
    size_t children_count;
} FILE;

// File access modes
#define FREAD  1
#define FWRITE 2
#define FREADWRITE 3
#define FAPPEND 8

// File attributes
#define FATTRIB_DIR 100

/// @brief Print formatted text to stdout
void printf(char *str, ...);