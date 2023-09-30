#ifndef __LOG_H__
#define __LOG_H__

#define LOGTYPE_SUCCESS 0
#define LOGTYPE_INFO 1
#define LOGTYPE_DBG 2
#define LOGTYPE_WARNING 3
#define LOGTYPE_ERROR 4

void log(int type, char *format, ...);

#endif