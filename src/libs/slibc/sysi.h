/*
 * @brief Header defining the System Interface structure.
 */
#ifndef __SYSI_H
#define __SYSI_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// @brief The System Interface, used to provide functions from the kernel.
typedef struct system_i {
    // @brief A pointer to the kernel PMM's allocate function
    void* (*malloc)(size_t);
    // @brief A pointer to the kernel PMM's free function
    void (*free)(void*);
} system_i;

#endif