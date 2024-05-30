#include <sipaa/fs/ramdisk.h>
#include <sipaa/bootsrv.h>
#include <sipaa/logger.h>
#include <sipaa/pmm.h>
#include <sipaa/libc/string.h>
#include <stdbool.h>
#include <limine.h>

FilesystemNodeT *RamDisk_Node;
TarHeaderT **RamDisk_Headers;
struct limine_file *RamDisk_File;
unsigned int RamDisk_HeaderCount;
bool RamDisk_Created = false;

unsigned int getsize(const char *in)
{
    unsigned int size = 0;
    unsigned int j;
    unsigned int count = 1;

    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);

    return size;
}

void RamDisk_Read(struct FilesystemNode* vnode, uint32_t offset, char* buffer, uint32_t count)
{
    if (vnode)
    {

    }
}

const char *ramdisk_get_file_content(const char *filename, unsigned int *size)
{
    for (unsigned int i = 0; i < RamDisk_HeaderCount; i++)
    {
        if (CompareStrings(RamDisk_Headers[i]->FileName, filename) == 0)
        {
            *size = getsize(RamDisk_Headers[i]->Size);
            return (const char *)((uintptr_t)RamDisk_Headers[i] + 512); // Add the header size to the header pointer. If changing the size (512), you will have total bull$hit!
        }
    }
    *size = 0;
    return NULL;
}

FilesystemNodeT *RamDisk_Create(unsigned int address)
{
    // Check if already created
    if (RamDisk_Created)
        return RamDisk_Node;

    // Get the ramdisk from limine
    RamDisk_File = BootSrv_GetModule(0);
    if (!RamDisk_File)
    {
        Log(LT_WARNING, "RamDisk", "No ramdisk available!\n");
        return NULL;
    }

    // Parse the ramdisk
    unsigned int i;

    for (i = 0;; i++)
    {
        TarHeaderT *header = (TarHeaderT *)address;

        if (header->FileName[0] == '\0')
            break;

        unsigned int size = getsize(header->Size);

        if (i == RamDisk_HeaderCount) {
            RamDisk_HeaderCount += 16; // increase by 16 each time
            RamDisk_Headers = Pmm_Reallocate(RamDisk_Headers, RamDisk_HeaderCount * sizeof(struct TarHeader*));
            if (!RamDisk_Headers) {
                // handle allocation error
                Log(LT_ERROR, "RamDisk", "Pmm_Reallocate failed! Halting system");
                for (;;) ;;
            }
        }

        RamDisk_Headers[i] = header;

        address += ((size / 512) + 1) * 512;

        if (size % 512)
            address += 512;
    }

    RamDisk_Headers = Pmm_Reallocate(RamDisk_Headers, i * sizeof(TarHeaderT*));

    // Create & return a file system node.
    RamDisk_Node = Pmm_Allocate(sizeof(FilesystemNodeT*));
    RamDisk_Node->Name = "ramdisk";
    RamDisk_Node->Read = RamDisk_Read;

    RamDisk_Created = true;

    return RamDisk_Node;
}