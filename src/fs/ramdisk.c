/*
#include <sipaa/logger.h>
#include <sipaa/pmm.h>
#include <sipaa/bootsrv.h>
#include <sipaa/memory.h>
#include <sipaa/libc/string.h>
#include <limine.h>
#include <tar.h>

struct limine_file *initrd = NULL;
struct ustar_header *tarfile = NULL;
unsigned int num_headers = 0;

int ramdisk_read_file(const char *filename, void *addr, size_t nbytes)
{
    unsigned int i;
    for (i = 0; ; i++)
    {
        struct TarHeader *header = headers[i];

        if (CompareStrings(header->filename, filename) == 0)
        {
            size_t size = (size_t)getsize(header->size);
            if (nbytes > size)
                return -2;
            
            void *ctaddr = initrd->address + (512 * i) + 512;

            memcpy(addr, (void *)ctaddr, nbytes);

            return size;
        }
    }

    return -1;
}

void RamDisk_Initialize()
{
    Log(LT_INFO, "RamDisk", "Trying getting ramdisk module...\n");

    // In SipaaKernel, the ramdisk should ALWAYS be the first module.
    initrd = BootSrv_GetModule(0);
    if (!initrd)
    {
        Log(LT_WARNING, "RamDisk", "No ramdisk present! Skipping the ramdisk initialization process...\n");
        return;
    }

    Log(LT_INFO, "RamDisk", "Parsing ramdisk...\n");

    tarfile = (struct ustar_header *)initrd->address;

    struct ustar_header *header = tarfile;
    while (strcmp(header->name, "./") != 0 || header->size[0] != '\0') {
        // Process file contents
        // ...
        Log(LT_INFO, "RamDisk", "File %s", hea)

        // Move to next header
        void *next_header_ptr = (void *)header + ALIGN(header->size, 512) + 512;
        header = (struct ustar_header *)next_header_ptr;
    }

    Log(LT_INFO, "RamDisk", "Ramdisk parsed!\n");
}
*/