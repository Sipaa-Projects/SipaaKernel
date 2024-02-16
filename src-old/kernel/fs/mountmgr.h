#pragma once

#define FST_DEVFS 1

int mount(const char* source, const char* target, unsigned int filesystem_type);
int umount(const char* target);