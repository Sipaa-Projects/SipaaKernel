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

void Scheduler_Initialize();
ProcessT *Scheduler_CreateProcess(char *name, int (*entry_point)());
void Scheduler_ExitProcess(ProcessT *proc);
void Scheduler_Schedule(RegistersT *regs);

extern void Scheduler_ContextSwitch(RegistersT *regs);

#endif