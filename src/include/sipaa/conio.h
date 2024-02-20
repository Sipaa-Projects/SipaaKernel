#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "skgbi.h"

#ifdef __cplusplus
extern "c" {
#endif

void conio_clear();
void conio_print(char *buf);
void conio_println(char *buf);
void conio_printf(char *format, ...);
void conio_printfln(char *format, ...);
void conio_vprintf(char *format, va_list args);
void conio_chbg(uint32_t bg);
void conio_chfg(uint32_t fg);
void conio_rstcol();
void conio_initialize(sk_general_boot_info skgbi);

#ifdef __cplusplus
}
#endif