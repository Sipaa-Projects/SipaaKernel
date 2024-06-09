#pragma once

#include <sipaa/fs/vfs.h>

#define EXT_FIFO 0x1000
#define EXT_CHAR_DEV 0x2000
#define EXT_DIRECTORY 0x4000
#define EXT_BLOCK_DEV 0x6000
#define EXT_FILE 0x8000
#define EXT_SYM_LINK 0xA000
#define EXT_UNIX_SOCKET 0xC000

#define EXT_MAX_CACHE 0x1024

typedef struct Ext2_Superblock {
  uint32_t inodes_count;
  uint32_t blocks_count;
  uint32_t su_resv_blocks_count;
  uint32_t free_blocks_count;
  uint32_t free_inodes_count;
  uint32_t block_num;
  uint32_t log2_block;
  uint32_t log2_frag;
  uint32_t blocks_per_group;
  uint32_t frags_per_group;
  uint32_t inodes_per_group;
  uint32_t last_mount_time;
  uint32_t last_write_time;
  uint16_t mount_times_check;
  uint16_t mount_times_allowed;
  uint16_t signature;
  uint16_t state;
  uint16_t err_handle;
  uint16_t minor_ver;
  uint32_t last_consistency_check;
  uint32_t consistency_interval;
  uint32_t os_id;
  uint32_t major_ver;
  uint16_t resv_blocks_user_id;
  uint16_t resv_blocks_group_id;
  uint32_t first_inode;
  uint16_t inode_size;
  uint16_t sb_bgd;
  uint32_t opt_features;
  uint32_t req_features;
  uint32_t mount_features;
  uint8_t fs_id[16];
  char vol_name[16];
  char vol_path_mount[64];
  uint32_t compression_algo;
  uint8_t preallocate_blocks_file;
  uint8_t preallocate_blocks_dir;
  uint16_t unused;
  uint64_t journal_id[2];
  uint32_t journal_inode;
  uint32_t journal_device;
  uint32_t orphan_inode_list;
  uint8_t unused_ext[787];
} Ext2_SuperblockT;

typedef struct Ext2_Inode {
  uint16_t type_perms;
  uint16_t user_id;
  uint32_t size;
  uint32_t last_access_time;
  uint32_t creation_time;
  uint32_t mod_time;
  uint32_t deletion_time;
  uint16_t group_id;
  uint16_t hard_link_count;
  uint32_t sector_count;
  uint32_t flags;
  uint32_t os_spec;
  uint32_t direct_block_ptr[12];
  uint32_t singly_block_ptr;
  uint32_t doubly_block_ptr;
  uint32_t triply_block_ptr;
  uint32_t gen_number;
  uint32_t file_acl;
  uint32_t dir_acl;
  uint32_t frag_block_addr;
  uint8_t os_spec2[12];
} Ext2_InodeT;

typedef struct Ext2_DirEntry {
  uint32_t inode;
  uint16_t total_size;
  uint8_t name_len;
  uint8_t type;
  //only if the feature bit for "directory entries have file type byte" is set, else this is the most-significant 8 bits of the Name Length
  uint8_t name[];
} Ext2_DirEntryT;

typedef struct Ext2_Bgd {
  uint32_t bitmap_block;
  uint32_t bitmap_inode;
  uint32_t inode_table_block;
  uint16_t free_blocks;
  uint16_t free_inodes;
  uint16_t directories_count;
  uint16_t pad;
  uint8_t resv[12];
} Ext2_BgdT;

typedef struct Ext2_Cache {
  uint32_t block;
  uint8_t* data;
} Ext2_CacheT;

typedef struct Ext2_Fs {
  Ext2_SuperblockT* sb;
  Ext2_BgdT* bgd_table;
  Ext2_CacheT* block_cache;
  Ext2_InodeT* root_ino;
  uint32_t block_size;
  uint32_t bgd_count;
  uint32_t bgd_block;
  uint32_t inode_size;
  uint32_t block_cache_idx;

  void(*read_block)(struct Ext2Fs* fs, uint32_t block, void* buf, uint32_t count);
} Ext2_FsT;

extern Ext2_FsT* root_fs;

uint8_t Ext2_Initialize();
FilesystemNodeT *Ext2_GetRootNode();

void Ext2_ReadBlock(Ext2_FsT* fs, uint32_t block, void* buf, uint32_t count);

void Ext2_ReadInode(Ext2_FsT* fs, uint32_t inode, Ext2_InodeT* in);
uint32_t Ext2_GetInode(Ext2_FsT* fs, Ext2_InodeT* in, char* name);
void Ext2_ReadInodeBlocks(Ext2_FsT* fs, Ext2_InodeT* in, uint8_t* buf, uint32_t count);
uint32_t Ext2_ReadFile(Ext2_FsT* fs, Ext2_InodeT* in, char* name, uint8_t* buf);

int Ext2_Read(FilesystemNodeT* vnode, uint8_t* buffer, uint32_t count);
DirectoryEntryT* Ext2_ReadDirectory(FilesystemNodeT* vnode, uint32_t index);
FilesystemNodeT* Ext2_FindDirectory(FilesystemNodeT* vnode, char* path);