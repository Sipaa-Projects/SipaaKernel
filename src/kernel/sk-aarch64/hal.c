// TODO: add support for disabling / enabling interrupts on aarch64
// TODO: initialize HAL on aarch64
// TODO: add support for relaxing CPU

#include <hal.h>
#include <sk-logger/logger.h>

int hal_init() { return 0; }

void hal_cpu_cli() { }
void hal_cpu_sti() { }

void hal_cpu_relax() { }
void hal_cpu_halt() 
{ 
    log(LT_INFO, "halting CPU\n");
    asm("wfi"); 
}

void hal_cpu_stop()
{
    while (1)
        hal_cpu_halt();
}