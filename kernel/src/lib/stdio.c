/*
 * stdio.c - Standard I/O functions.
 *
 * This file is part of the SipaaKernel's standard C library.
 */
#include <lib/stdio.h>

#include <dev/serial/serial.h>
#include <memory/memory.h>
#include <lib/log.h>
#include <lib/string.h>
#include <lib/terminal.h>
#include <stdarg.h>

file_descriptor *fd_table[MAX_FD];
bool is_init = false;

void finit()
{
    for (int i = 0; i < MAX_FD; i++) {
        fd_table[i] = malloc(sizeof(file_descriptor));
        file_descriptor *fd = fd_table[i];
        fd->mode = NULL;
        fd->type = -1;
        fd->address = NULL;
        fd->used = 0;
    }

    is_init = true;
}

int fopen(const char *filename, const char *mode) {
    if (!is_init)
        finit();

    file_descriptor *fd;
    int fdint;
    bool found_fd = false;

    for (int i = 0; i < MAX_FD; i++) {
        fd = fd_table[i];

        if (fd->used != 0)
            continue;

        found_fd = true;
        fd->mode = strdup(mode);
        fd->used = true;
        fdint = i;
        break;
    }

    if(found_fd)
    {
        // Since SipaaKernel doesn't have a proper file system right now, we will compare strings for returning the appropriate component.
        if (strcmp(filename, "/dev/ttyS0") == 0) // Serial device
        { 
            fd->type = FILE_TYPE_DEV;
            fd->address = serial_init();
        }
        else if (strcmp(filename, "/dev/tty0") == 0) // Terminal device
        { 
            fd->type = FILE_TYPE_DEV;
            fd->address = terminal_getdev();
        }

        if (!fd->address) {
            return -1;  // File not found
        }

        return fdint;
    }

    return -2;  // No available file descriptor
}

size_t fwrite(const char *ptr, size_t size, size_t count, int fd) {
    file_descriptor *fd2 = fd_table[fd];

    if (fd < 0 || fd >= MAX_FD)
        return -1;  // Invalid file descriptor

    if (!fd2->address)
        return -2;

    if (!strstr(fd2->mode, "w"))
        return -3;

    if (fd2->type == FILE_TYPE_DEV)
    {
        dev *device = (dev *)fd2->address;
        device->write(device, ptr, size * count);
        return count;
    }

    return 0;
}

/* private methods for fprintf */
size_t fprint_integer(int fd, int v, int base,
    unsigned char * digits) {
    char buf[33];
    char * ptr = & buf[sizeof(buf) - 1];
    * ptr = '\0';

    if (v == 0) {
        return fwrite('0', sizeof(char), 1, fd);
    }

    if (v < 0 && base == 10) {
        fwrite('-', sizeof(char), 1, fd);
        v = -v;
    }

    while (v) {
        *--ptr = digits[v % base];
        v /= base;
    }

    return fwrite(ptr, sizeof(char), sizeof(ptr), fd);
}

size_t fprint_integer_64(int fd, ui64 v, int base,
    unsigned char * digits) {
    char buf[65];
    char * pointer = & buf[sizeof(buf) - 1];
    * pointer = '\0';

    if (v == 0) {
        return fwrite('0', sizeof(char), 1, fd);
    }

    while (v) {
        *--pointer = digits[v % base];
        v /= base;
    }

    return fwrite(pointer, sizeof(char), sizeof(pointer), fd);
}
/* end of private methods for fprintf */

size_t fprintf(int fd, char *format, ...)
{
    va_list args;
    va_start(args, format);

    size_t total_size = 0;

    const char *hex_digits = "0123456789ABCDEF";
    char *ifs = "Invalid format specifier";

    while (*format != '\0')
    {
        if (*format == '%') {
            format++;
            switch (*format)
            {
            case 's':
            {
                char *str = va_arg(args, char *);
                total_size += fwrite(str, sizeof(char), strlen(str), fd);
                break;
            }
            case 'c':
            {
                char c = (char)va_arg(args, int);
                total_size += fwrite(&c, sizeof(char), 1, fd);
                break;
            }
            case 'd':
            {
                int d = va_arg(args, int);
                fprint_integer(fd, d, 10, hex_digits);
                break;
            }
            case 'u':
            {
                unsigned int u = va_arg(args, unsigned int);
                fprint_integer(fd, u, 10, hex_digits);
                break;
            }
            case 'x':
            {
                int x = va_arg(args, int);
                fprint_integer(fd, x, 16, hex_digits);
                break;
            }
            case 'p':
            {
                void *p = va_arg(args, void *);
                fprint_integer_64(fd, (ui64)p, 16, hex_digits);
                break;
            }
            case 'l':
            {
                format++;
                if (*format == 'l')
                {
                    format++;
                    if (*format == 'u')
                    {
                        ui64 llu = va_arg(args, ui64);
                        fprint_integer_64(fd, llu, 10, hex_digits);
                    }
                    else if (*format == 'x')
                    {
                        ui64 llx = va_arg(args, ui64);
                        fprint_integer_64(fd, llx, 16, hex_digits);
                    }
                    else
                    {
                        fwrite(ifs, sizeof(char), strlen(ifs), fd);
                    }
                }
                else
                {
                    fwrite(ifs, sizeof(char), strlen(ifs), fd);
                }
                break;
            }
            default:
                char c = '%';
                fwrite(&c, sizeof(char), 1, fd);
                fwrite(format, sizeof(char), 1, fd);
                break;
            }
        }
        else
        {
            total_size += fwrite(format, sizeof(char), 1, fd);
        }
        format++;
    }

    va_end(args);
}

size_t fread(char *ptr, size_t size, size_t count, int fd) {
    file_descriptor *fd2 = fd_table[fd];

    if (fd < 0 || fd >= MAX_FD || fd2->address == NULL || !strstr(fd2->mode, "r")) {
        return 0;  // Invalid file descriptor
    }

    if (fd2->type == FILE_TYPE_DEV)
    {
        dev *device = (dev *)fd2->address;
        device->read(device, ptr, size * count);
        return count;
    }

    return 0;
}

int fclose(int fd)
{
    // Validate the file descriptor
    if (fd < 0 || fd >= MAX_FD) {
        return -1;  // Invalid file descriptor
    }

    file_descriptor *fd_entry = fd_table[fd];
    
    // Check if the file descriptor is actually open
    if (fd_entry == NULL || fd_entry->used == 0) {
        return -2;  // File descriptor not in use
    }

    // If the file type is a device, you might want to close it here.
    // This will depend on your device handling logic.
    if (fd_entry->type == FILE_TYPE_DEV) {
        // Free the device
        free(fd_entry->address);
    }

    // Release resources (like memory allocated for mode)
    if (fd_entry->mode) {
        free(fd_entry->mode);
        fd_entry->mode = NULL;
    }

    // Mark the file descriptor as unused
    fd_entry->used = 0;
    fd_entry->type = -1;
    fd_entry->address = NULL;

    return 0;  // Successfully closed
}