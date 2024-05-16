/// @brief HelloFS is a small "filesystem" that serves as an example to use the SipaaKernel's VFS.

#include <sipaa/fs/vfs.h>
#include <sipaa/pmm.h>

FilesystemNodeT *hellofs_node;
bool created = false;

char *hello = NULL;
size_t hello_size = 6; // The size of the hello string, including the null terminator

void HelloFS_Read(struct FilesystemNode* vnode, uint32_t offset, uint32_t count, uint8_t* buffer)
{

}

FilesystemNodeT *HelloFS_Create()
{
    if (hellofs_node || created)
        return NULL;

    hello = Pmm_Allocate(hello_size);

    hellofs_node = (FilesystemNodeT *)Pmm_Allocate(sizeof(FilesystemNodeT));
    hellofs_node->Read = HelloFS_Read;
    hellofs_node->Size = hello_size;

    return hellofs_node;
}