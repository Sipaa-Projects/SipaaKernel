#ifndef PIT_H
#define PIT_H

#include <stdint.h>
#include <sipaa/x86_64/io.h>

#define PIT_FREQUENCY 1193182
#define HZ 1000

extern uint32_t tick;

void Pit_Initialize(uint32_t frequency);
void Sleep(uint32_t seconds);
void SleepMs(uint32_t milliseconds);
void SleepUs(uint32_t milliseconds);

#endif