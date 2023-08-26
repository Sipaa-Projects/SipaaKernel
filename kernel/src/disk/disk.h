#pragma once

#include <arch/io.h>
#include <memory/memory.h>
#include <lib/lib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define ATA_PRIMARY_IO 0x1F0
#define ATA_REG_DATA ATA_PRIMARY_IO
#define ATA_REG_SECTORCOUNT ATA_PRIMARY_IO + 2
#define ATA_REG_LBA_LO ATA_PRIMARY_IO + 3
#define ATA_REG_LBA_MID ATA_PRIMARY_IO + 4
#define ATA_REG_LBA_HI ATA_PRIMARY_IO + 5
#define ATA_REG_DRIVESEL ATA_PRIMARY_IO + 6
#define ATA_REG_COMMAND ATA_PRIMARY_IO + 7
#define ATA_CMD_READ_PIO 0x20
#define ATA_CMD_WRITE_PIO 0x30

#define ATA_REG_DATA 0x1F0
#define ATA_REG_ERROR 0x1F1
#define ATA_REG_COUNT 0x1F2
#define ATA_REG_LBA1 0x1F3
#define ATA_REG_LBA2 0x1F4
#define ATA_REG_LBA3 0x1F5
#define ATA_REG_DRIVESEL 0x1F6
#define ATA_REG_STATUS 0x1F7
#define ATA_REG_COMMAND 0x1F7

#define PRIMARY_BUS_IO_BASE 0x1F0
#define SECONDARY_BUS_IO_BASE 0x170

#define ATA_CMD_IDENTIFY_PACKET 0xA1
#define ATA_CMD_READ_SECTORS_EXT 0xA8

namespace Sk {
namespace Disk {

typedef struct DirectoryEntry
{
        uint8_t name[11];
        uint8_t attr;
        uint8_t nt_res;
        uint8_t create_time_tenth;
        uint16_t create_time;
        uint16_t create_date;
        uint16_t last_access_date;
        uint16_t cluster_high;
        uint16_t last_modif_time;
        uint16_t last_modif_date;
        uint16_t cluster_low;
        uint32_t size;
} __attribute__((packed));

typedef struct BootSector
{
        uint8_t jump[3];
        uint8_t oem_name[8];
        uint16_t bytes_per_sector;
        uint8_t sectors_per_cluster;
        uint16_t reserved_sector_count;
        uint8_t fat_count;
        uint16_t root_dir_entries;
        uint16_t total_sectors_short;
        uint8_t media_type;
        uint16_t table_size_16;
        uint16_t sectors_per_track;
        uint16_t head_side_count;
        uint32_t hidden_sector_count;
        uint32_t total_sectors_long;
        uint32_t table_size_32;
        uint16_t ext_flags;
        uint16_t fat_version;
        uint32_t root_cluster;
        uint16_t fs_info_sector;
        uint16_t backup_boot_sector;
        uint8_t reserved[12];
        uint8_t drive_number;
        uint8_t reserved1;
        uint8_t boot_signature;
        uint32_t volume_id;
        uint8_t volume_label[11];
        uint8_t fs_type[8];
        uint8_t boot_code[420];
        uint16_t boot_sector_signature;
} __attribute__((packed));

enum DiskFormat {
    FAT16,
    FAT32,
    ISO9660,
    UNKNOWN
};

class DiskUtil {
public:
    static BootSector *BootSector;
    static uint32_t CurrentDirectoryCluster;
    static char CurrentPath[256];

    static bool AtapiIdentify();
    static void AtapiReadSector(uint32_t lba, void *buffer);
    static void ReadSpecificSector(uint8_t bus, bool is_master, uint32_t sector, void *buffer);
    static void FormatFilename83(const char *input, char *output);
    static void FormatEntryName(const char *entry_name, char *formatted_name);
    static void WaitForDriveReady();
    static void ReadSector(uint32_t sector, void *buffer);
    static void WriteSector(uint32_t sector, void *buffer);
    static uint32_t ClusterToSector(uint32_t cluster, struct BootSector *_bs);
    static DiskFormat GetDiskFormat(uint32_t sector);
};

}
}
