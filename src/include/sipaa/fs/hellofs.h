#pragma once

#include <sipaa/fs/vfs.h>

FilesystemNodeT *HelloFS_Create();
void HelloFS_Read(struct FilesystemNode* vnode, uint32_t offset, uint32_t count, uint8_t* buffer);