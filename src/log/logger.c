// SipaaKernel implementation for the logger
#include <sipaa/logger.h>
#include <sipaa/dev/conio.h>
#include <sipaa/spinlock.h>
#include <stdint.h>
#include <sipaa/kdebug.h>

#define SK_SHOWDBGLINES

int logger_enabled = 1; // 1 : true, 0 : false
int logger_enableserial = 1;
int logger_enableconio = 1;
SpinlockT logger_lock = SPINLOCK_INIT;

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

    if (logger_enableconio)
        ConIO_Print(buf);

    if (debugger_ready && logger_enableserial)
        Dbg_Print(buf);
}

void shared_print(char *buf)
{
    if (logger_enableconio)
        ConIO_Print(buf);

    if (debugger_ready && logger_enableserial)
        Dbg_Print(buf);
}

void shared_chfg(enum LogType type)
{
    if (logger_enableconio)
        ConIO_SetFg(graphicalLineColors[type]);

    if (debugger_ready && logger_enableserial)
        Dbg_Print(lineColors[type]);
}

void shared_rstcol()
{
    if (logger_enableconio)
        ConIO_ResetColor();

    if (debugger_ready && logger_enableserial)
        Dbg_Print("\033[0m");
}

void __internal_log(char *file, char *line, enum LogType type, char *message, ...)
{
    if (logger_enabled != 1 && type != LT_FATAL)
        return;

    #ifndef SK_SHOWDBGLINES
    if (type == LT_DEBUG)
        return;
    #endif

    //Spinlock_Acquire(&logger_lock);

    shared_chfg(type);
    shared_print(file); // "file" isn't the source file, but the component!
    shared_rstcol();
    shared_print(": ");

    va_list args;
    va_start(args, message);

    shared_vprintf(message, args);

    va_end(args);

    //Spinlock_Release(&logger_lock);
}