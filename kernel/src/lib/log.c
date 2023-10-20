#include "log.h"

#include "terminal.h"
#include <stdarg.h>

void print_type(int type)
{
    if (type == LOGTYPE_DBG)
    {
        current_terminal->fgcolor = 0xA1D4BF;
        print_string("Debug");
    }
    else if (type == LOGTYPE_SUCCESS)
    {
        current_terminal->fgcolor = 0xBD00FF;
        print_string("Success");
    }
    else if (type == LOGTYPE_INFO)
    {
        current_terminal->fgcolor = 0x00FFEC;
        print_string("Info");
    }
    else if (type == LOGTYPE_WARNING)
    {
        current_terminal->fgcolor = 0xFFF500;
        print_string("Warning");
    }
    else if (type == LOGTYPE_ERROR)
    {
        current_terminal->fgcolor = 0xE90606;
        print_string("Error");
    }
    else
    {
        current_terminal->fgcolor = 0xFFFFFF;
        print_string("Unknown");
    }

    current_terminal->fgcolor = 0xFFFFFF;
}

void log(int type, char *format, ...)
{
    va_list args;
    va_start(args, format);

    print_character('[');
    print_type(type);
    print_character(']');
    print_character(' ');
    vprintf(format, args);

    va_end(args);
}