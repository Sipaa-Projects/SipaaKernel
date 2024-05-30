/// @brief SipaaKernel's VFS
#pragma once

#include <stdint.h>
#include <stdbool.h>

// Path separator
#define FS_PATHSEP '/'
#define FS_PATHSEP_STR "/"
#define FS_PATHUP ".."
#define FS_PATHCURRENT "."

// File types
#define FS_TYPE_DIRECTORY 1
#define FS_TYPE_FILE 2
#define FS_TYPE_DEVICE 3

// Permissions
#define FS_PERM_DESTROY 1

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

  /// @brief Returned when trying to set a new root filesystem
  VFS_NOT_ALLOWED = -6,

  /// @brief Returned when a VFS operation is not supported or implemented
  VFS_NOT_SUPPORTED = -7,
};

typedef enum VfsStatus VfsStatusT;


struct DirectoryEntry {
  char* Name;
  uint32_t Inode;
};

typedef struct DirectoryEntry DirectoryEntryT;


struct FilesystemNode {
  char* Name;
  struct FilesystemNode* Parent;
  bool Open;
  uint32_t Permissions;
  uint32_t Type;
  uint32_t Size;
  uint32_t Inode;
  int(*Read)(struct FilesystemNode* vnode, uint8_t *buffer, uint32_t count);
  int(*Write)(struct FilesystemNode* vnode, uint8_t *buffer, uint32_t count);
  DirectoryEntryT*(*ReadDirectory)(struct FilesystemNode* vnode, uint32_t index);
  struct FilesystemNode*(*FindDirectory)(struct FilesystemNode* vnode, char* path);
};

typedef struct FilesystemNode FilesystemNodeT;


typedef struct Directory {
  FilesystemNodeT* node;
  uint32_t Index;
} DirectoryT; // Used mainly for syscalls


struct Vfs_MountPoint {
    const char* mount_path;
    FilesystemNodeT *node;
};

typedef struct Vfs_MountPoint Vfs_MountPointT;

extern FilesystemNodeT *vfs_root;

// FUNCTIONS

/// @brief Set the VFS' root node.
VfsStatusT Vfs_SetRootNode(FilesystemNodeT *node);

int Vfs_Write(FilesystemNodeT* node, uint8_t* buffer, uint32_t count);
int Vfs_Read(FilesystemNodeT* node, uint8_t* buffer, uint32_t count);
DirectoryEntryT* Vfs_ReadDirectory(FilesystemNodeT* node, uint32_t index);
FilesystemNodeT* Vfs_FindDirectory(FilesystemNodeT* node, char* path);
FilesystemNodeT* Vfs_Open(FilesystemNodeT* node, char* path); // traverse directories
void Vfs_Destroy(FilesystemNodeT* node);