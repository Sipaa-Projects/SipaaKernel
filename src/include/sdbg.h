#pragma once

#include <stdint.h>

#define SDBG_MAGIC_1 'S'
#define SDBG_MAGIC_2 'D'
#define SDBG_MAGIC_3 'B'
#define SDBG_MAGIC_4 'G'
#define SDBG_MAGIC_5 '1'
#define SDBG_MAGIC_STR "SDBG1"
#define SDBG_MAGIC_SIZE 5

// Legal values for SDBGHeader.symtype
#define SDBG_SYMTYPE_MAINHEADER -1
#define SDBG_SYMTYPE_NOTYPE 0
#define SDBG_SYMTYPE_OBJECT 1
#define SDBG_SYMTYPE_FUNCTION 2
#define SDBG_SYMTYPE_SECTION 3
#define SDBG_SYMTYPE_FILE 4

typedef struct SDBGHeader
{
    char magic[5];
    int symcount;
    int symtype;
    int symsize;
    uint64_t symval;
    uint64_t symendval; // symval + symsize
    char symname[256];
} SDBGHeaderT;
