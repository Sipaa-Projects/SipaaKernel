#pragma once

#include <stdint.h>

#define SYSCALL_TABLE_SIZE 256

struct Syscall_Arguments
{
    void *Argument1;
    void *Argument2;
    void *Argument3;
    void *Argument4;
    void *Argument5;
    void *Argument6;
};
typedef struct Syscall_Arguments Syscall_ArgumentsT;

typedef void *(*Syscall_Function)(Syscall_ArgumentsT);

extern Syscall_Function syscall_table[SYSCALL_TABLE_SIZE];

void Syscall_Initialize();