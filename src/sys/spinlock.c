/*
 * @file src/sys/spinlock.c
 * @brief Spinlock implementation
 */
#include <sipaa/spinlock.h>
#include <sipaa/logger.h>

__attribute__((noinline)) int Spinlock_Acquire(SpinlockT *lock) {
    #ifdef __x86_64__
    volatile size_t deadlock_counter = 0;
    for (;;) {
        if (Spinlock_TestAndAcquire(lock)) {
            break;
        }
        if (++deadlock_counter >= 100000000) {
            Log(LT_ERROR, "spinlock", "resource not available!\n");
            return -1;
        }
#if defined (__x86_64__)
        __asm__ volatile ("pause");
#endif
    }
    lock->last_acquirer = __builtin_return_address(0);
    return 0;
    #endif
}