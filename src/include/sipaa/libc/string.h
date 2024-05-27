#pragma once

#include <stddef.h>

// TODO: StringDuplicate cannot be implemented due to the heap memory requirement.

int CompareStrings(const char *str1, const char *str2);
char *StringFromString(char *s1, char *s2);
int StringLength(char *s);
void StringRemove(char *str, char *sub);
void CopyStringN(char* dst, const char* src, size_t n);
char *CharFromString(const char *s, int c);