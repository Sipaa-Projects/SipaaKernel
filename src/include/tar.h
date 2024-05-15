/*
 * tar.h
 */
#ifndef _TAR_H
#define _TAR_H

/* General definitions */
#define TMAGIC "ustar" /* ustar plus null byte. */
#define TMAGLEN 6      /* Length of the above. */
#define TVERSION "00"  /* 00 without a null byte. */
#define TVERSLEN 2     /* Length of the above. */
/* Typeflag field definitions */
#define REGTYPE '0'   /* Regular file. */
#define AREGTYPE '\0' /* Regular file. */
#define LNKTYPE '1'   /* Link. */
#define SYMTYPE '2'   /* Symbolic link. */
#define CHRTYPE '3'   /* Character special. */
#define BLKTYPE '4'   /* Block special. */
#define DIRTYPE '5'   /* Directory. */
#define FIFOTYPE '6'  /* FIFO special. */
#define CONTTYPE '7'  /* Reserved. */
/* Mode field bit definitions (octal) */
#define TSUID 04000   /* Set UID on execution. */
#define TSGID 02000   /* Set GID on execution. */
#define TSVTX 01000   /* On directories, restricted deletion flag. */
#define TUREAD 00400  /* Read by owner. */
#define TUWRITE 00200 /* Write by owner. */
#define TUEXEC 00100  /* Execute/search by owner. */
#define TGREAD 00040  /* Read by group. */
#define TGWRITE 00020 /* Write by group. */
#define TGEXEC 00010  /* Execute/search by group. */
#define TOREAD 00004  /* Read by other. */
#define TOWRITE 00002 /* Write by other. */
#define TOEXEC 00001  /* Execute/search by other. */

struct ustar_header
{
    char name[100];     /* File name.  Null-terminated if room. */
    char mode[8];       /* Permissions as octal string. */
    char uid[8];        /* User ID as octal string. */
    char gid[8];        /* Group ID as octal string. */
    char size[12];      /* File size in bytes as octal string. */
    char mtime[12];     /* Modification time in seconds
                           from Jan 1, 1970, as octal string. */
    char chksum[8];     /* Sum of octets in header as octal string. */
    char typeflag;      /* An enum ustar_type value. */
    char linkname[100]; /* Name of link target.
                           Null-terminated if room. */
    char magic[6];      /* "ustar\0" */
    char version[2];    /* "00" */
    char uname[32];     /* User name, always null-terminated. */
    char gname[32];     /* Group name, always null-terminated. */
    char devmajor[8];   /* Device major number as octal string. */
    char devminor[8];   /* Device minor number as octal string. */
    char prefix[155];   /* Prefix to file name.
                           Null-terminated if room. */
    char padding[12];   /* Pad to 512 bytes. */
    void *file_contents; /* Pointer to the file's contents. */
    struct ustar_header *next_header; /* Pointer to the next TAR header. */
};

#endif