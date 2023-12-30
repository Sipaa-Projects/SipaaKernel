#include "slibc-internals.h"
#include <logger/logger.h>
#include <stdbool.h>

bool initialized = false;

void slibc_initialize()
{
    log(LT_INFO, "slibc", "Initializing...");
    initialized = true;
    log(LT_INFO, "slibc", "Initialized!");
}