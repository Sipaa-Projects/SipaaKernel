/// @brief SipaaKernel's VFS
#pragma once

#include <stdint.h>

// Path separator
#define FS_PATHSEP '/'
#define FS_PATHSEP_STR "/"
#define FS_PATHUP "."
#define FS_PATHCURRENT "."

// File types
#define FS_TYPE_DIRECTORY 0
#define FS_TYPE_FILE 0
#define FS_TYPE_CHARDEV 1
#define FS_TYPE_IODEV 1

enum VfsStatus {
  /// @brief Returned when succeded.
  VFS_SUCCESS = 0,

  /// @brief Returned when the provided FS node is NULL
  VFS_NULL_NODE = -4,

  /// @brief Returned when the mountpoint list is full.
  VFS_SLOTS_FULL = -3,

  /// @brief Returned when a filesystem is already mounted on the provided path
  VFS_SLOT_ALREADY_TAKEN = -2,

  /// @brief Returned when trying to unmount the filesystem.
  VFS_CANNOT_UNMOUNT_ROOT = -5,

  /// @brief Returned when trying to unmount an inexistant mount point.
  VFS_NOT_MOUNTED = -1,
};

typedef enum VfsStatus VfsStatusT;


struct DirectoryEntry {
  char* Name;
  uint32_t Inode;
};

typedef struct DirectoryEntry DirectoryEntryT;


struct FilesystemNode {
  char* Name;
  uint32_t Permissions;
  uint32_t Type;
  uint32_t Size;
  uint32_t Inode;
  uint32_t(*Read)(struct FilesystemNode* vnode, uint32_t offset, uint32_t count, uint8_t* buffer);
  DirectoryEntryT*(*ReadDirectory)(struct FilesystemNode* vnode, uint32_t index);
  struct FilesystemNode*(*FindDirectory)(struct FilesystemNode* vnode, char* path);
};

typedef struct FilesystemNode FilesystemNodeT;


struct Vfs_MountPoint {
    const char* mount_path;
    FilesystemNodeT *node;
};

typedef struct Vfs_MountPoint Vfs_MountPointT;


// FUNCTIONS

/// @brief Initialize the VFS' root node.
void Vfs_Initialize();

/// @brief Mount a filesystem on a path
/// @param target The target path (exemple: /dev)
/// @param node The corresponding node (exemple: a DevFS node containing devices)
/// @return A status indicating the success or an error.
VfsStatusT Vfs_Mount(const char* target, FilesystemNodeT *node);


/// @brief Unmount a filesystem
/// @param target A path where a file system is mounted
/// @return A status indicating the success or an error.
VfsStatusT Vfs_Unmount(const char* target);