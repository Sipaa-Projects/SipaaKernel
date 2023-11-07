#include <stddef.h>

#include "pmm.h"
#include "panic.h"
#include <entry.h>
#include <serial.h>
#include <flanterm/flanterm.h>
#include <flanterm/backend-fb.h>
#include <sk-hal/hal.h>
#include <sk-logger/logger.h>

extern void logger_sk_impl_init(); // Defined in src/kernel/sk-core/logger-sk-impl.c at line 44

void _start(void)
{   
    serial_init();
    logger_sk_impl_init();
    pmm_init();

    if (hal_init() != 0)
        panic("HAL initialization failed");

    hal_time *current_time = pmm_alloc(sizeof(hal_time));
    hal_get_time(current_time);

    log(LT_INFO, "Current RTC date & time: %d/%d/%d %d:%d:%d\n", 
            current_time->year, current_time->month, current_time->day_of_month, current_time->hours, current_time->minutes, current_time->seconds);

    hal_cpu_stop();
}