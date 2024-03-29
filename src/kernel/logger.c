// SipaaKernel implementation for the logger
#include <sipaa/logger.h>
#include <sipaa/conio.h>
#include <stdint.h>
#include <sipaa/kdebug.h>

int logger_enabled = 1; // 1 : true, 0 : false

/// @brief The line colors for the serial console.
char *lineColors[LT_LENGTH] = {
    "\033[36m",
    "\033[33m",
    "\033[31m"};

/// @brief The line colors for the graphical console.
uint32_t graphicalLineColors[LT_LENGTH] = {
    0x03b6fc,
    0xfcc203,
    0xff0000};

char *lineStarts[LT_LENGTH] = {
    "Info =>",
    "Warn =>",
    "Error =>"};

/*
const char *deftermcolor = "";

char *lineColors[LT_LENGTH] = {
    "\033[36m",
    "\033[33m",
    "\033[31m"
};
*/

void logger_setenabled(int enabled)
{
    logger_enabled = enabled;
}

int logger_isenabled()
{
    return logger_enabled;
}

/// @brief A function to print formatted text to the 2 output streams (ConIO & SerialIO). Runs 2x faster than doing conio_printf & dbg_printf
void shared_vprintf(char *format, va_list args)
{
    char buf[32767];

    npf_vsnprintf(buf, sizeof(buf), format, args);

    conio_print(buf);

    if (debugger_ready)
        dbg_print(buf);
}

void shared_print(char *buf)
{
    conio_print(buf);

    if (debugger_ready)
        dbg_print(buf);
}

void shared_chfg(enum LogType type)
{
    conio_chfg(graphicalLineColors[type]);

    if (debugger_ready)
        dbg_print(lineColors[type]);
}

void shared_rstcol()
{
    conio_rstcol();

    if (debugger_ready)
        dbg_print("\033[0m");
}

void __internal_log(char *file, char *line, enum LogType type, char *message, ...)
{
    if (logger_enabled != 1)
        return;

    shared_chfg(type);
    shared_print(lineStarts[type]);
    shared_rstcol();
    shared_print(" ");
    shared_print(file);
    shared_print(" ");
    shared_print(line);
    shared_print(" => ");

    va_list args;
    va_start(args, message);

    shared_vprintf(message, args);

    va_end(args);
}

void log_nofileinfo(enum LogType type, char *message, ...)
{
    if (logger_enabled != 1)
        return;

    conio_chfg(graphicalLineColors[type]);
    conio_print(lineStarts[type]);
    conio_rstcol();
    conio_print(" => ");
    conio_print(message);
}