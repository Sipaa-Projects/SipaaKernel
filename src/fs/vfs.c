#include <stdint.h>
#include <stddef.h>
#include <sipaa/libc/string.h>
#include <sipaa/logger.h>
#include <sipaa/fs/vfs.h>
#include <sipaa/heap.h>

// Define structure for mounted filesystem

FilesystemNodeT *vfs_root;

VfsStatusT Vfs_SetRootNode(FilesystemNodeT *node)
{
    if (node != NULL) {
        vfs_root = node;
        return VFS_SUCCESS;
    }
    return VFS_NULL_NODE;
}

void Vfs_TryFindFS()
{
    
}

int Vfs_Write(FilesystemNodeT *node, uint8_t *buffer, uint32_t count)
{
    if (!node)
        return VFS_NULL_NODE;

    if (node->Write)
        return node->Write(node, buffer, count);

    return VFS_NOT_SUPPORTED;
}

int Vfs_Read(FilesystemNodeT *node, uint8_t *buffer, uint32_t count)
{
    if (!node)
        return VFS_NULL_NODE;

    if (node->Read)
        return node->Read(node, buffer, count);

    return VFS_NOT_SUPPORTED;
}

DirectoryEntryT *Vfs_ReadDirectory(FilesystemNodeT *node, uint32_t index)
{
    if (!node)
        return NULL;

    if (node->ReadDirectory && node->Type == FS_TYPE_DIRECTORY)
        return node->ReadDirectory(node, index);

    return NULL;
}

FilesystemNodeT *Vfs_FindDirectory(FilesystemNodeT *node, char *path)
{
    if (!node)
        return NULL;

    if (node->FindDirectory && node->Type == FS_TYPE_DIRECTORY)
        return node->FindDirectory(node, path);

    return NULL;
}

FilesystemNodeT *Vfs_Open(FilesystemNodeT *node, char *path)
{
    if (CompareStringsN(path, FS_PATHCURRENT, 1))
        return node;

    if (CompareStringsN(path, FS_PATHUP, 2))
        return node->Parent;
    
    bool vfs_root = (path[0] == FS_PATHSEP_STR);
    int plen = StringLength(path);

    bool has_subdir = false;
    for (int i = (vfs_root ? 1 : 0); i < plen; i++)
        if (path[i] == FS_PATHSEP_STR)
        {
            has_subdir = true;
            break; // it has subdirs
        }

    if (!has_subdir)
        return Vfs_FindDirectory((vfs_root ? vfs_root : node), path + (vfs_root ? 1 : 0));

    char *_path = (char *)KHeap_Allocate(plen);
    memcpy(_path, (vfs_root ? path + 1 : path), plen);

    char *token = TokenizeString(_path, FS_PATHSEP_STR);
    FilesystemNodeT *current = (vfs_root ? vfs_root : node);

    while (token)
    {
        current = Vfs_FindDirectory(current, token);
        if (current == NULL)
        {
            KHeap_Free(_path);
            return NULL;
        }

        token = TokenizeString(NULL, FS_PATHSEP_STR);
    }
    KHeap_Free(_path);

    return current;
}

void Vfs_Destroy(FilesystemNodeT *vnode)
{
    if (!(vnode->Permissions & FS_PERM_DESTROY) || vnode->Open)
        return;

    KHeap_Free(vnode->Name);
    KHeap_Free(vnode);
}