#include <stdint.h>
#include <stddef.h>
#include <slibc/string.h>
#include <logger/logger.h>
#include <fs/devfs/devfs.h>
#include "mountmgr.h"

#define PATHSEP '/'

// Define structure for mounted filesystem
typedef struct mount_point {
    const char* mount_path;
    void* filesystem;  // Generic pointer for any filesystem
    uint32_t filesystem_type;  // Type identifier for the filesystem
} mount_point;

mount_point mount_points[50];

int mount(const char* source, const char* target, uint32_t filesystem_type) {
    if (filesystem_type == FST_DEVFS) {
        // Mount devfs
        FILE* devfs = get_main_node();
        if (devfs == NULL) {
            log(LT_ERROR, "mountmgr", "cannot mount '%s' on '%s': devfs is NULL.\n", source, target);
            return -1;
        }
        
        // Check if the target path is available
        for (size_t i = 0; i < sizeof(mount_points) / sizeof(mount_points[0]); ++i) {
            if (mount_points[i].mount_path != NULL && strcmp(mount_points[i].mount_path, target) == 0) {
                log(LT_ERROR, "mountmgr", "cannot mount '%s' on '%s': A filesystem is already mounted on the target directory.\n", source, target);
                return -2;
            }
        }
        
        size_t empty_slot = -1;
        for (size_t i = 0; i < sizeof(mount_points) / sizeof(mount_points[0]); ++i) {
            if (mount_points[i].mount_path == NULL) {
                empty_slot = i;
                break;
            }
        }
        
        if (empty_slot == -1) {
            log(LT_ERROR, "mountmgr", "cannot mount '%s' on '%s': no available slots\n", source, target)
            return -3;
        }
        
        mount_points[empty_slot].mount_path = target;
        mount_points[empty_slot].filesystem = devfs;

        // TODO: Perform any additional setup or initialization as needed for the specific filesystem

        log(LT_INFO, "mountmgr", "successfully mounted '%s' on '%s'\n", source, target);
        return 0;
    }

    log(LT_ERROR, "mountmgr", "cannot mount '%s' on '%s': unknown file system type.\n", source, target);
    return -4;
}

/// @brief Unmount a file system
/// @param target The directory where the FS is mounted
/// @return 
int umount(const char* target) {
    // Find the mount point based on the target path
    for (size_t i = 0; i < sizeof(mount_points) / sizeof(mount_points[0]); ++i) {
        if (mount_points[i].mount_path != NULL && strcmp(mount_points[i].mount_path, target) == 0) {
            // TODO: Perform any cleanup or finalization as needed for the specific filesystem
            if (target == "/") // check if the file path is '/', aka the root directory.
            {
                log(LT_ERROR, "mountmgr", "Cannot unmount the root file system. If it was unmounted, it would cause system stability issues and data loss.\n");
                return;
            }

            mount_points[i].mount_path = NULL;
            mount_points[i].filesystem = NULL;

            return 0;  // Unmount successful
        }
    }

    return -1;
}
