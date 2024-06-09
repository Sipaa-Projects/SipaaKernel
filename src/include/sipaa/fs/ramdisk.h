#pragma once

#include <stdint.h>
#include <sipaa/fs/vfs.h>

struct TarHeader
{
    char FileName[100];
    char Mode[8];
    char Uuid[8];
    char Guid[8];
    char Size[12];
    char ModifcationTime[12];
    char Checksum[8];
    char Typeflag[1];
};
typedef struct TarHeader TarHeaderT;

/// @brief Creates a file system node for accessing the ramdisk
/// @return 
FilesystemNodeT *RamDisk_Create();
unsigned int ramdisk_parse_headers();
void ramdisk_list_files();
const char* ramdisk_get_file_content(const char *filename, unsigned int *size);