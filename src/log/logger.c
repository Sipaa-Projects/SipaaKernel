// SipaaKernel implementation for the logger
#include <sipaa/logger.h>
#include <sipaa/drv/conio.h>
#include <stdint.h>
#include <sipaa/kdebug.h>

#define SK_SHOWDBGLINES

int logger_enabled = 1; // 1 : true, 0 : false

/// @brief The line colors for the serial console.
char *lineColors[LT_LENGTH] = {
    "\033[36m",
    "\033[33m",
    "\033[31m"
    "\033[35m",
    "\033[30m",
    "\033[34m",};

/// @brief The line colors for the graphical console.
uint32_t graphicalLineColors[LT_LENGTH] = {
    0x03b6fc,
    0xfcc203,
    0xff0000,
    0xB200DE,
    0x505050,
    0x00DF86};

char *lineStarts[LT_LENGTH] = {
    "Info =>",
    "Warn =>",
    "Error =>",
    "Success =>",
    "Fatal =>",
    "Debug =>"};

/*
const char *deftermcolor = "";

char *lineColors[LT_LENGTH] = {
    "\033[36m",
    "\033[33m",
    "\033[31m"
};
*/

void Logger_SetEnabled(int enabled)
{
    logger_enabled = enabled;
}

int Logger_IsEnabled()
{
    return logger_enabled;
}

/// @brief A function to print formatted text to the 2 output streams (ConIO & SerialIO). Runs 2x faster than doing conio_printf & Dbg_printf
void shared_vprintf(char *format, va_list args)
{
    char buf[32767];

    npf_vsnprintf(buf, sizeof(buf), format, args);

    ConIO_Print(buf);

    if (debugger_ready)
        Dbg_Print(buf);
}

void shared_print(char *buf)
{
    ConIO_Print(buf);

    if (debugger_ready)
        Dbg_Print(buf);
}

void shared_chfg(enum LogType type)
{
    ConIO_SetFg(graphicalLineColors[type]);

    if (debugger_ready)
        Dbg_Print(lineColors[type]);
}

void shared_rstcol()
{
    ConIO_ResetColor();

    if (debugger_ready)
        Dbg_Print("\033[0m");
}

void __internal_log(char *file, char *line, enum LogType type, char *message, ...)
{
    if (logger_enabled != 1)
        return;

    #ifndef SK_SHOWDBGLINES
    if (type == LT_DEBUG)
        return;
    #endif

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