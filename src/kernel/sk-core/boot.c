#include <stddef.h>

#include "pmm.h"
#include "panic.h"
#include <entry.h>
#include <serial.h>
#include <flanterm/flanterm.h>
#include <flanterm/backend-fb.h>
#include <sk-hal/hal.h>
#include <sk-logger/logger.h>

/// @brief The pointer to 'logger_sk_impl_init', defined in logger-impl-sk.c
extern void logger_sk_impl_init();

/// @brief The kernel entry point. It's the first function called when Limine loads a kernel who uses the LBP.
void _start()
{   
    serial_init();
    logger_sk_impl_init();
    log(LT_INFO, " _____ _                   _   __                     _ \n");
    log(LT_INFO, "/  ___(_)                 | | / /                    | |\n");
    log(LT_INFO, "\\ `--. _ _ __   __ _  __ _| |/ /  ___ _ __ _ __   ___| |\n");
    log(LT_INFO, " `--. \\ | '_ \\ / _` |/ _` |    \\ / _ \\ '__| '_ \\ / _ \\ |\n");
    log(LT_INFO, "/\\__/ / | |_) | (_| | (_| | |\\  \\  __/ |  | | | |  __/ |\n");
    log(LT_INFO, "\\____/|_| .__/ \\__,_|\\__,_\\_| \\_/\\___|_|  |_| |_|\\___|_|\n");
    log(LT_INFO, "        | |                                             \n");
    log(LT_INFO, "        |_|                                             \n");
    log(LT_INFO, "                                                        \n");
    log(LT_INFO, "             - Copyright (C) 2023-present Sipaa Projects\n");
    pmm_init();

    if (hal_init() != 0)
        panic("HAL initialization failed");

    hal_time *current_time = pmm_alloc(sizeof(hal_time));
    hal_get_time(current_time);

    log(LT_INFO, "Current RTC date & time: %d/%d/%d %d:%d:%d\n", 
            current_time->year, current_time->month, current_time->day_of_month, current_time->hours, current_time->minutes, current_time->seconds);

    hal_cpu_stop();
}