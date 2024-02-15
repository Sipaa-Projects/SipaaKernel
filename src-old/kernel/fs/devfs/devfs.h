#pragma once

#include <stdint.h>
#include <stddef.h>
#include <slibc/stdio.h>

/**typedef struct devfs_node {
    const char* name;
    uint16_t permissions;
    uint32_t type;
    void (*read)(char* buffer, size_t size, size_t offset);
    void (*write)(const char* buffer, size_t size, size_t offset);
} devfs_node;

typedef struct devfs_directory_node {
    const char* name;
    uint16_t permissions;
    size_t child_count;
    devfs_node **child_nodes;
} devfs_directory_node;**/

FILE* get_main_node();
FILE* devfs_get_node(const char* name);
void devfs_add_device(FILE *node);
void devfs_remove_device(FILE *node);