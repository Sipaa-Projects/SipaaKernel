/// @brief HelloFS is a small "filesystem" that serves as an example to use the SipaaKernel's VFS.

#include <sipaa/fs/vfs.h>
#include <sipaa/pmm.h>

FilesystemNodeT *hellofs_node;
bool created = false;

char *hello = NULL;
size_t hello_size = 6; // The size of the hello string, including the null terminator

void HelloFS_Read(struct FilesystemNode* vnode, uint32_t offset, uint8_t* buffer, uint32_t count)
{
    if (count > (hello_size - offset))
        buffer = NULL;

    if (buffer == NULL)
        return;


    for (int i = offset; i < count; i++)
    {
        buffer[i] = hello[i];
    } 
}

FilesystemNodeT *HelloFS_Create()
{
    if (hellofs_node || created)
        return NULL;

    hello = Pmm_Allocate(hello_size);
    *hello = "Hello"; 

    hellofs_node = (FilesystemNodeT *)Pmm_Allocate(sizeof(FilesystemNodeT));
    hellofs_node->Read = HelloFS_Read;
    hellofs_node->Size = hello_size;

    return hellofs_node;
}