/*
 *  printf.c - 'printf' function implementation.
 *  
 *  Copyright (C) 2023-present Sipaa Projects.
 */
#include "../stdio.h"
#include <logger/logger.h>

void printf(char *text, ...)
{
    log(LT_WARNING, "slibc", "Some component made a call to %s, which isn't implemented right now.\n", __FUNCTION__);
}