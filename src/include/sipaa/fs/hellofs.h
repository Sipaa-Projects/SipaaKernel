#pragma once

FilesystemNodeT *HelloFS_Create();
void HelloFS_Read(struct FilesystemNode* vnode, UI32 offset, UI32 count, UI8* buffer);