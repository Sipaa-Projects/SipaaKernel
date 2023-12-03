#ifndef LOGGING_H
#define LOGGING_H

#include <stdarg.h>

#define LOG_ERROR 1
#define LOG_WARN 2
#define LOG_INFO 3
#define LOG_DEBUG 4

void log(int level, const char *message, ...);

#endif