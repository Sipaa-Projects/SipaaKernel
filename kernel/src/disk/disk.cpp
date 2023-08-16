#include "disk.h"

using namespace Sk::Memory;
using namespace Sk::Arch;

namespace Sk {
namespace Disk {

struct BootSector *DiskUtil::BootSector;
uint32_t DiskUtil::CurrentDirectoryCluster;
char DiskUtil::CurrentPath[256] = "/";

void DiskUtil::ReadSector(uint32_t sector, void *buffer)
{
    Io::OutB(ATA_REG_DRIVESEL, 0xE0 | ((sector & 0x0F000000) >> 24));
    Io::OutB(ATA_REG_SECTORCOUNT, 1);
    Io::OutB(ATA_REG_LBA_LO, sector & 0xFF);
    Io::OutB(ATA_REG_LBA_MID, (sector & 0xFF00) >> 8);
    Io::OutB(ATA_REG_LBA_HI, (sector & 0xFF0000) >> 16);
    Io::OutB(ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    while ((Io::InB(ATA_REG_COMMAND) & 0x08) == 0)
        ;

    Io::InSW(ATA_REG_DATA, buffer, 256);
}

void DiskUtil::WriteSector(uint32_t sector, void *buffer)
{
    Io::OutB(ATA_REG_DRIVESEL, 0xE0 | ((sector & 0x0F000000) >> 24));
    Io::OutB(ATA_REG_SECTORCOUNT, 1);
    Io::OutB(ATA_REG_LBA_LO, sector & 0xFF);
    Io::OutB(ATA_REG_LBA_MID, (sector & 0xFF00) >> 8);
    Io::OutB(ATA_REG_LBA_HI, (sector & 0xFF0000) >> 16);
    Io::OutB(ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);

    while ((Io::InB(ATA_REG_COMMAND) & 0x08) == 0)
        ;

    Io::OutSW(ATA_REG_DATA, buffer, 256);
}

void DiskUtil::WaitForDriveReady()
{
    while (Io::InB(ATA_REG_STATUS) & 0x80)
        ;
}

bool DiskUtil::AtapiIdentify()
{
    Io::OutB(ATA_REG_DRIVESEL, 0xA0);
    Io::OutB(ATA_REG_COUNT, 0);
    Io::OutB(ATA_REG_LBA1, 0);
    Io::OutB(ATA_REG_LBA2, 0);
    Io::OutB(ATA_REG_LBA3, 0);

    Io::OutB(ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);

    WaitForDriveReady();

    if (Io::InB(ATA_REG_STATUS) & 0x01)
        return false;

    uint16_t data[256];
    for (int i = 0; i < 256; i++)
        data[i] = Io::InW(ATA_REG_DATA);

    return true;
}

void DiskUtil::AtapiReadSector(uint32_t lba, void *buffer)
{
    Io::OutB(ATA_REG_DRIVESEL, 0xA0);
    Io::OutB(ATA_REG_COUNT, 1);
    Io::OutB(ATA_REG_LBA1, lba & 0xFF);
    Io::OutB(ATA_REG_LBA2, (lba >> 8) & 0xFF);
    Io::OutB(ATA_REG_LBA3, (lba >> 16) & 0xFF);

    Io::OutB(ATA_REG_COMMAND, ATA_CMD_READ_SECTORS_EXT);

    WaitForDriveReady();

    Io::InSW(ATA_REG_DATA, buffer, 256);
}

void DiskUtil::ReadSpecificSector(uint8_t bus, bool is_master, uint32_t sector, void *buffer)
{
    uint16_t io_base = bus == 0 ? PRIMARY_BUS_IO_BASE : SECONDARY_BUS_IO_BASE;
    uint8_t drive_select = is_master ? 0xA0 : 0xB0;

    Io::OutB(io_base + ATA_REG_DRIVESEL, drive_select | ((sector & 0x0F000000) >> 24));
    Io::OutB(io_base + ATA_REG_SECTORCOUNT, 1);
    Io::OutB(io_base + ATA_REG_LBA_LO, sector & 0xFF);
    Io::OutB(io_base + ATA_REG_LBA_MID, (sector & 0xFF00) >> 8);
    Io::OutB(io_base + ATA_REG_LBA_HI, (sector & 0xFF0000) >> 16);
    Io::OutB(io_base + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    while ((Io::InB(io_base + ATA_REG_COMMAND) & 0x08) == 0)
        ;

    Io::InSW(io_base + ATA_REG_DATA, buffer, 256);
}

void DiskUtil::FormatFilename83(const char *input, char *output)
{
    BasicMemoryManagement::MemorySet32(output, ' ', 11);
    int i, j;

    for (i = 0, j = 0; i < 8 && input[j] != '.' && input[j] != '\0'; i++, j++)
    {
        output[i] = Lib::ToUpper(input[j]);
    }

    while (input[j] != '.' && input[j] != '\0')
    {
        j++;
    }

    if (input[j] == '.')
    {
        j++;
        for (i = 8; i < 11 && input[j] != '\0'; i++, j++)
        {
            output[i] = Lib::ToUpper(input[j]);
        }
    }
    output[11] = '\0';
}

void DiskUtil::FormatEntryName(const char *entry_name, char *formatted_name)
{
    int i, j;

    for (i = 0, j = 0; i < 8 && entry_name[j] != ' '; j++)
    {
        formatted_name[i++] = Lib::ToLower(entry_name[j]);
    }

    if (entry_name[8] != ' ')
    {
        formatted_name[i++] = '.';
    }

    for (j = 8; j < 11 && entry_name[j] != ' '; j++)
    {
        formatted_name[i++] = Lib::ToLower(entry_name[j]);
    }
    
    formatted_name[i] = '\0';
}

uint32_t DiskUtil::ClusterToSector(uint32_t cluster, struct BootSector *_bs)
{
    uint32_t first_sector_of_cluster_2 = _bs->reserved_sector_count + (_bs->fat_count * _bs->table_size_32);

    return (cluster - 2) * _bs->sectors_per_cluster + first_sector_of_cluster_2;
}

DiskFormat DiskUtil::GetDiskFormat(uint32_t sector)
{
    uint8_t buffer[512];
    ReadSector(sector, buffer);

    if (BasicMemoryManagement::MemoryCompare(&buffer[1], "CD001", 5) == 0)
    {
        return ISO9660;
    }

    uint16_t *fat_signature = (uint16_t *)(&buffer[510]);
    if (*fat_signature == 0xAA55)
    {
        if (BasicMemoryManagement::MemoryCompare(&buffer[82], "FAT32", 5) == 0)
        {
            return FAT32;
        }
        else if (BasicMemoryManagement::MemoryCompare(&buffer[54], "FAT", 3) == 0)
        {
            return FAT16;
        }
    }

    return UNKNOWN;
}

}
}
