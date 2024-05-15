#include <stdint.h>
#include <stddef.h>
#include <sipaa/libc/string.h>
#include <sipaa/logger.h>
#include <sipaa/fs/vfs.h>

// Define structure for mounted filesystem

Vfs_MountPointT mount_points[50];

int Vfs_Mount(const char *target, FilesystemNodeT *node)
{
    if (node)
    {
        // Check if the target path is available
        for (size_t i = 0; i < sizeof(mount_points) / sizeof(mount_points[0]); ++i)
        {
            if (mount_points[i].mount_path != NULL && CompareStrings(mount_points[i].mount_path, target) == 0)
            {
                Log(LT_ERROR, "Vfs", "Cannot mount '%s' on '%s': A filesystem is already mounted on the target directory.\n", node->Name, target);
                return VFS_SLOT_ALREADY_TAKEN;
            }
        }

        size_t empty_slot = -1;
        for (size_t i = 0; i < sizeof(mount_points) / sizeof(mount_points[0]); ++i)
        {
            if (mount_points[i].mount_path == NULL)
            {
                empty_slot = i;
                break;
            }
        }

        if (empty_slot == -1)
        {
            Log(LT_ERROR, "Vfs", "Cannot mount '%s' on '%s': no available slots\n", node->Name, target);
            return VFS_SLOTS_FULL;
        }

        mount_points[empty_slot].mount_path = target;
        mount_points[empty_slot].filesystem = devfs;

        // TODO: Perform any additional setup or initialization as needed for the specific filesystem

        Log(LT_INFO, "Vfs", "Successfully mounted '%s' on '%s'\n", node->Name, target);
        return VFS_SUCCESS;
    }

    Log(LT_ERROR, "Vfs", "Cannot mount '%s' on '%s': unknown file system type.\n", node->Name, target);
    return VFS_NULL_NODE;
}

int Vfs_Unmount(const char *target)
{
    // Find the mount point based on the target path
    for (size_t i = 0; i < sizeof(mount_points) / sizeof(mount_points[0]); ++i)
    {
        if (mount_points[i].mount_path != NULL && strcmp(mount_points[i].mount_path, target) == 0)
        {
            // TODO: Perform any cleanup or finalization as needed for the specific filesystem
            if (target == FS_PATHSEP_STR) // check if the file path is equal to the path separator, aka the root directory.
            {
                Log(LT_ERROR, "Vfs", "Cannot unmount the root file system. If it was unmounted, it would cause system stability issues and data loss.\n");
                return VFS_CANNOT_UNMOUNT_ROOT;
            }

            mount_points[i].mount_path = NULL;
            mount_points[i].filesystem = NULL;

            return VFS_SUCCESS; // Unmount successful
        }
    }

    return VFS_NOT_MOUNTED;
}