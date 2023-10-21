/*
 * stdio.h - Standard I/O functions. (header)
 *
 * This file is part of the SipaaKernel's standard C library.
 */
#ifndef __STDIO_H__
#define __STDIO_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <lib/stdtype.h>
#include <dev/device.h>

#define MAX_FD 1000

#define FILE_TYPE_DEV 0
#define FILE_TYPE_FILE 1

typedef struct {
    void *address;
    char *mode;
    int type;
    ui32 used;
    // Add other fields like mode, current position, etc.
} file_descriptor;

int fopen(const char *filename, const char *mode);
size_t fwrite(const char *ptr, size_t size, size_t count, int fd);
size_t fprintf(int fd, char *format, ...);
size_t fread(char *ptr, size_t size, size_t count, int fd);
int fclose(int fd);

#endif