#pragma once

#include <stddef.h>
#include <stdint.h>
#include <entry/skgbi.h>

void conio_write(char *buf, size_t charsize, size_t len);
void conio_chbg(uint32_t bg);
void conio_chfg(uint32_t fg);
void conio_rstcol();
void conio_initialize(sk_general_boot_info skgbi);