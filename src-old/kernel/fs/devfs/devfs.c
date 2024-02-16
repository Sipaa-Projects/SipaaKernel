#include <logger/logger.h>
#include <slibc/string.h>
#include "devfs.h"

// Array to store device nodes
FILE devfs_nodes[] = {
};

FILE devfs_empty_file = {
    .handle = -4
};

// Current position in the 'devfs_nodes' array
int buf_pos = 0;

FILE devfs_main_directory = {
    .attribs = FATTRIB_DIR,
    .handle = 0, // this handle should be unique to devfs.
    .children_count = 0,
    .children = &devfs_nodes
};

FILE* devfs_get_node(const char* name) {
    for (size_t i = 0; i < sizeof(devfs_nodes) / sizeof(devfs_nodes[0]); ++i) {
        if (strcmp(devfs_nodes[i].name, name) == 0) {
            return &devfs_nodes[i];
        }
    }
    return NULL;
}

FILE* get_main_node()
{
    log(LT_INFO, "devfs", "Main file has been requested!\n");
    return &devfs_main_directory;
}

void devfs_add_device(FILE *node)
{
    devfs_main_directory.children[devfs_main_directory.children_count] = node;
    devfs_main_directory.children_count++;
    log(LT_INFO, "devfs", "Device '%s' has been added to the FS.\n", node->name);
}

void devfs_remove_device(FILE *node)
{
    for (size_t i = 0; i < sizeof(devfs_nodes) / sizeof(devfs_nodes[0]); ++i) {
        if (strcmp(devfs_nodes[i].name, node->name) == 0) {
            devfs_main_directory.children[i] = &devfs_empty_file;
            devfs_main_directory.children_count--;
            log(LT_INFO, "devfs", "Device '%s' has been removed from the FS.\n", devfs_main_directory.children[i]->name);
        }
    }
}