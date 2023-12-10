// SipaaKernel implementation for the logger
#include <logger/logger.h>
#include <device/serial/serial.h>

int logger_enabled = 1; // 1 : true, 0 : false

void (*logger_write)(enum LogType, int, char *, int); // (logtype, isLogTypeText, text, size)

char *lineStarts[LT_LENGTH] = {
    "[ INFO ]",
    "[ WARNING ]",
    "[ ERROR ]"
};

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

int __internal_logger_strlen(char *str)
{
    if (!str)
        return 0;

    int i = 0;

    while (str[i] != '\0')
        i++;

    return i;
}

void __internal_logger_print_i(int v, int base, const char *digits) {
    char buf[33];
    char * ptr = & buf[sizeof(buf) - 1];
    char zero = '0';
    char unk = '-';
    * ptr = '\0';

    if (v == 0) {
        logger_write(LT_LENGTH, 0, &zero, 1);
        return;
    }

    if (v < 0 && base == 10) {
        logger_write(LT_LENGTH, 0, &unk, 1);
        v = -v;
    }

    while (v) {
        *--ptr = digits[v % base];
        v /= base;
    }

    logger_write(LT_LENGTH, 0, ptr, __internal_logger_strlen(ptr));
}

void __internal_logger_print_i64(uint64_t v, int base, const char *digits) {
    char buf[65];
    char *pointer = & buf[sizeof(buf) - 1];
    char zero = '0';
    char unk = '-';
    *pointer = '\0';

    if (v == 0) {
        logger_write(LT_LENGTH, 0, &zero, 1);
        return;
    }

    while (v) {
        *--pointer = digits[v % base];
        v /= base;
    }

    logger_write(LT_LENGTH, 0, pointer, __internal_logger_strlen(pointer));
}

int __logger_internal_strlen(char *str)
{
    if (!str)
        return 0;
    
    int length = 0;

    while (*str != '\0')
    {
        length++;
        str++;
    }

    return length;
}

void __logger_internal_vprintf(char *format, va_list args)
{
        const char *hex_digits = "0123456789ABCDEF";

        while (*format != '\0')
        {
                if (*format == '%')
                {
                        format++;
                        switch (*format)
                        {
                        case 's':
                        {
                                char *str = va_arg(args, char *);
                                logger_write(LT_LENGTH, 0, str, __logger_internal_strlen(str));
                                break;
                        }
                        case 'c':
                        {
                                char c = (char)va_arg(args, int);
                                logger_write(LT_LENGTH, 0, &c, 1);
                                break;
                        }
                        case 'd':
                        {
                                int d = va_arg(args, int);
                                __internal_logger_print_i(d, 10, hex_digits);
                                break;
                        }
                        case 'u':
                        {
                                unsigned int u = va_arg(args, unsigned int);
                                __internal_logger_print_i(u, 10, hex_digits);
                                break;
                        }
                        case 'x':
                        {
                                int x = va_arg(args, int);
                                __internal_logger_print_i(x, 16, hex_digits);
                                break;
                        }
                        case 'p':
                        {
                                void *p = va_arg(args, void *);
                                __internal_logger_print_i64((uint64_t)p, 16, hex_digits);
                                break;
                        }
                        case 'l':
                        {
                                format++;
                                if (*format == 'l')
                                {
                                        format++;
                                        if (*format == 'u')
                                        {
                                                uint64_t llu = va_arg(args, uint64_t);
                                                __internal_logger_print_i64(llu, 10, hex_digits);
                                        }
                                        else if (*format == 'x')
                                        {
                                                uint64_t llx = va_arg(args, uint64_t);
                                                __internal_logger_print_i64(llx, 16, hex_digits);
                                        }
                                        else
                                        {
                                                logger_write(LT_LENGTH, 0, "Invalid format specifier", sizeof("Invalid format specifier"));
                                        }
                                }
                                else
                                {
                                        logger_write(LT_LENGTH, 0, "Invalid format specifier", sizeof("Invalid format specifier"));
                                }
                                break;
                        }
                        default:
                                logger_write(LT_LENGTH, 0, "(F)", sizeof("(F)"));
                                logger_write(LT_LENGTH, 0, format, 1);
                                break;
                        }
                }
                else
                {
                        logger_write(LT_LENGTH, 0, format, 1);
                }
                format++;
        }
}

__attribute__((no_caller_saved_registers)) void __internal_log(char *file, char *line, enum LogType type, char *message, ...) {
    if (logger_enabled != 1)
        return;
    logger_write(type, 1, lineStarts[type], __internal_logger_strlen(lineStarts[type]));
    logger_write(LT_LENGTH, 0, " ", 1);
    logger_write(LT_LENGTH, 0, file, __internal_logger_strlen(file));
    logger_write(LT_LENGTH, 0, " ln", 1);
    logger_write(LT_LENGTH, 0, line, __internal_logger_strlen(line));
    logger_write(LT_LENGTH, 0, ": ", 2);
    va_list args;
    va_start(args, message);
    __logger_internal_vprintf(message, args);
    va_end(args);
}

void log_nofileinfo(enum LogType type, char *message, ...) {
    if (logger_enabled != 1)
        return;
    logger_write(type, 1, lineStarts[type], __internal_logger_strlen(lineStarts[type]));
    logger_write(LT_LENGTH, 0, " ", 1);
    va_list args;
    va_start(args, message);
    __logger_internal_vprintf(message, args);
    va_end(args);
}