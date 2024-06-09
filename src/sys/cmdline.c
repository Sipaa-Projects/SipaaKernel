/*
 * @file src/sys/cmdline.c
 * @brief Command line parser
 */
#include <sipaa/libc/string.h>
#include <stddef.h>

void CmdLine_GetOption(char *key, char *value, char *token) {
    char *p = CharFromString(token, '=');
    if (p != NULL) {
        *p++ = '\0';
        if (CompareStrings(key, token) == 0) {
            CopyStringN(value, p, sizeof(value));
        }
    }
}