#pragma once

#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>

#define SPINLOCK_INIT {0, NULL}

typedef struct Spinlock {
    int Lock;
    void *last_acquirer;
} SpinlockT;


static inline bool Spinlock_TestAndAcquire(SpinlockT *lock) {
    return __sync_bool_compare_and_swap(&lock->Lock, 0, 1);
}

int Spinlock_Acquire(SpinlockT *lock);

static inline void Spinlock_Release(SpinlockT *lock) {
    lock->last_acquirer = NULL;
    __atomic_store_n(&lock->Lock, 0, __ATOMIC_SEQ_CST);
}