#include <sipaa/logger.h>
#include <sipaa/libc/string.h>
#include <sipaa/fs/vfs.h>
//#include <sipaa/fs/devfs.h>
#include <stddef.h>

// Array to store device nodes
FilesystemNodeT *devfs_nodes[] = {
};
int devfs_nodes_count = 0;

FilesystemNodeT devfs_empty_file = {
    .Name = "\0"
};

FilesystemNodeT devfs_main_directory = {
    .Type = FS_TYPE_DIRECTORY,
    .Inode = -6869867083,
};

FilesystemNodeT* devfs_get_node(const char* name) {
    for (size_t i = 0; i < sizeof(devfs_nodes) / sizeof(devfs_nodes[0]); ++i) {
        if (CompareStrings(devfs_nodes[i]->Name, name) == 0) {
            return &devfs_nodes[i];
        }
    }
    return NULL;
}

FilesystemNodeT* get_main_node()
{
    Log(LT_INFO, "devfs", "Main file has been requested!\n");
    return &devfs_main_directory;
}

void devfs_add_device(FilesystemNodeT *node)
{
    devfs_nodes[devfs_nodes_count] = node;
    devfs_nodes_count++;
    Log(LT_INFO, "DevFS", "Device '%s' has been added to the FS.\n", node->Name);
}

void devfs_remove_device(FilesystemNodeT *node)
{
    for (size_t i = 0; i < sizeof(devfs_nodes) / sizeof(devfs_nodes[0]); ++i) {
        if (CompareStrings(devfs_nodes[i]->Name, node->Name) == 0) {
            devfs_nodes[i] = &devfs_empty_file;
            devfs_nodes_count--;
            Log(LT_INFO, "DevFS", "Device '%s' has been removed from the FS.\n", node->Name);
        }
    }
}