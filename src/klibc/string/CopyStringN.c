#include <sipaa/libc/string.h>

void CopyStringN(char* dst, const char* src, size_t n)
{
   size_t i = 0;
   while (i++ != n && (*dst++ = *src++));
}