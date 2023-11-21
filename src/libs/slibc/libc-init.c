/*
 * @brief File used to initialize the libc
 */
#include "sysi.h"

/// @brief A pointer to the current system interface.
system_i *current_sysi;

/// @brief Init the LibC, required for most of the functions.
/// @param sysin A pointer to the system interface.
/// @returns true if succeded, else false.
bool libc_init(system_i *sysin)
{
    if (sysin)
    {
        current_sysi = sysin;
        return true;
    }
    return false;
}