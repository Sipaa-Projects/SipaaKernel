// TODO: add support for disabling / enabling interrupts on aarch64
// TODO: initialize HAL on aarch64
// TODO: add support for relaxing CPU

#include <hal.h>
#include <sk-logger/logger.h>

/// @brief Initialize the HAL (aarch64 specific) (nothing to initialize)
/// @return 0 if success, else error
int hal_init() { return 0; }

/// @brief Disable interrupts (aarch64 specific) (not implemented)
void hal_cpu_cli() { }

/// @brief Enable interrupts (aarch64 specific) (not implemented)
void hal_cpu_sti() { }

/// @brief Relax/pause CPU (aarch64 specific) (not implemented)
void hal_cpu_relax() { }

/// @brief Halt the CPU (aarch64 specific)
void hal_cpu_halt() 
{ 
    log(LT_INFO, "halting CPU\n");
    asm("wfi"); 
}

/// @brief Disable interrupts then halt CPU (aarch64 specific) (partially implemented)
void hal_cpu_stop()
{
    while (1)
        hal_cpu_halt();
}