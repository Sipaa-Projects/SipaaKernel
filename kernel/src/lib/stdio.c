#include <lib/stdio.h>

#include <dev/serial/serial.h>
#include <memory/memory.h>
#include <lib/log.h>
#include <lib/string.h>

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

        if (!fd->address) {
            return -1;  // File not found
        }

        return fdint;
    }

    return -2;  // No available file descriptor
}

size_t fwrite(const void *ptr, size_t size, size_t count, int fd) {
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

size_t fread(void *ptr, size_t size, size_t count, int fd) {
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