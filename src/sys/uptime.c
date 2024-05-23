/*
 * @file src/sys/uptime.c
 * @brief System uptime counter.
 * 
 * This uses a CPU core to compute the system uptime.
 * TODO: This is running too fast! Slow it down using the RTC.
 */
#include <sipaa/uptime.h>
#include <sipaa/logger.h>
#include <sipaa/bootsrv.h>
#include <stdatomic.h>
#include <stdbool.h>

UI32 ctr = 0;
UI32 uptime_s = 0;
UI32 uptime_ms = 0;
bool UptimeCtr_Initialized = false;

void UptimeCounter_Entry(struct limine_smp_info *info) {
    Log(LT_INFO, "UptimeCtr", "Hello world! From now, i will count the system uptime.\n");
    UptimeCtr_Initialized = true;
    
    while (1)
    {
        if (uptime_ms == 9999)
        {
            __atomic_fetch_add(&uptime_s, 1, __ATOMIC_SEQ_CST);
            uptime_ms = 0;
            continue;
        }
        __atomic_fetch_add(&uptime_ms, 1, __ATOMIC_SEQ_CST);
    }
}

UI32 UptimeCounter_GetSeconds() { return uptime_s; }
UI32 UptimeCounter_GetMilliseconds() { return uptime_ms; }

void UptimeCounter_Initialize()
{
    struct limine_smp_info *cpu2 = BootSrv_GetCPU(1);

    if (cpu2) {
        UI32 old_ctr = __atomic_load_n(&ctr, __ATOMIC_SEQ_CST);

        __atomic_store_n(&cpu2->goto_address, UptimeCounter_Entry, __ATOMIC_SEQ_CST);

        __atomic_load_n(&ctr, __ATOMIC_SEQ_CST);

        // Wait for the uptime counter to be initialized, so we don't confuse the logger with 2 log calls at the same time.
        while (!UptimeCtr_Initialized)
            ;;

        return;
    }
    Log(LT_WARNING, "UptimeCtr", "Unfortunately, there's not 2 CPUs or more.\n");
}