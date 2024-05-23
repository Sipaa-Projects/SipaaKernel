/*
 * @file src/sys/uptime.c
 * @brief System uptime counter header.
 * 
 * This uses a CPU core to compute the system uptime.
 * TODO: This is running too fast! Slow it down using the RTC.
 */
#include <sipaa/libc/types.h>

/// @brief Gets the ellapsed seconds
/// @return The ellapsed seconds
UI32 UptimeCounter_GetSeconds();

/// @brief Gets the millisecond count
/// @return The millisecond count
/// @remark The counter is reset when it reaches 1000.
UI32 UptimeCounter_GetMilliseconds();

/// @brief Setup the second CPU to run the uptime counter
void UptimeCounter_Initialize();