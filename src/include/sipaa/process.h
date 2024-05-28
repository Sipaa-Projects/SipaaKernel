#ifndef TASKING_H
#define TASKING_H

#include <sipaa/x86_64/idt.h>

#define PROCESS_NAME_SIZE 128

typedef struct Process
{
    char name[PROCESS_NAME_SIZE];
    RegistersT regs;
    uint64_t stack_ptr;
    struct Process *next;
    uint64_t *pml4;
    int pid;
} ProcessT;

extern ProcessT *current_process;
extern ProcessT *process_list;

void Process_Init();
ProcessT *Process_Create(char *name, int (*entry_point)());
void Process_Exit(ProcessT *proc);
void Schedule(RegistersT *regs);

extern void Process_ContextSwitch(RegistersT *regs);

#endif