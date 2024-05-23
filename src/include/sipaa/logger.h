#ifndef __LOGGER_H
#define __LOGGER_H

#include <stdarg.h>

enum LogType {
    LT_INFO,
    LT_WARNING,
    LT_ERROR,
    LT_SUCCESS,
    LT_FATAL,
    LT_DEBUG,
    LT_LENGTH
};

#ifdef __cplusplus
extern "c" {
#endif

void __internal_log(char *file, char *line, enum LogType type, char *message, ...);

void Logger_SetEnabled(int enabled);
int Logger_IsEnabled();

#ifdef __cplusplus
}
#endif

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define Log(type, cmp, message, ...) { __internal_log(cmp, STRINGIZE(__LINE__), type, message, ##__VA_ARGS__); }

#endif