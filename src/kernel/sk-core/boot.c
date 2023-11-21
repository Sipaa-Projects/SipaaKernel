#include <stddef.h>

#include "pmm.h"
#include "panic.h"
#include <entry.h>
#include <serial.h>
#include <flanterm/flanterm.h>
#include <flanterm/backend-fb.h>
#include <sk-hal/hal.h>
#include <sk-logger/logger.h>
#include <sysi.h>
#include <slibc/stdlib.h>

/// @brief The pointer to 'logger_sk_impl_init', defined in logger-impl-sk.c
extern void logger_sk_impl_init();

/// @brief Defined in src/libs/slibc/libc-init.c, line 12
extern bool libc_init(system_i *sysin);

/// @brief Initialize SLibC
bool _libc_sk_init()
{
    system_i *sksysi = pmm_alloc(sizeof(system_i));
    sksysi->malloc = pmm_alloc;
    sksysi->free = pmm_free;

    return libc_init(sksysi);
}

/// @brief Test the libc
/// @return true if success, else false
bool _libc_test() {
    void *ptr = malloc(100);
    if (ptr)
    {
        log(LT_INFO, "Successfully allocated memory block of size '100' at 0x%p using SLibC\n", ptr);
        free(ptr);
        return true;
    }
    log(LT_WARNING, "Could not allocate a memory block using SLibC.\n");
    return false;
}

/// @brief The kernel entry point. It's the first function called when Limine loads a kernel who uses the LBP. I prefer to call this the "prekernel", as it loads only core components.
void _start()
{   
    bool r = false;

    serial_init();
    pmm_init();
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
    log(LT_INFO, "      - Copyright (C) August 2022-present Sipaa Projects\n");

    if (hal_init() != 0)
        panic("HAL initialization failed");

    r = _libc_sk_init();
    if (r == false)
       panic("The C standard library initialization failed.\n");

    r = _libc_test();
    if (r == false) {
       panic("LibC test failed.\n");
    }
    else {
       log(LT_INFO, "LibC test succeded.\n");
    }


    hal_time *current_time = pmm_alloc(sizeof(hal_time));
    hal_get_time(current_time);

    log(LT_INFO, "Current RTC date & time: %d/%d/%d %d:%d:%d\n", 
            current_time->year, current_time->month, current_time->day_of_month, current_time->hours, current_time->minutes, current_time->seconds);

    hal_cpu_stop();
}