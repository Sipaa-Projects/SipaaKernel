#include <sipaa/x86_64/vmm.h>
#include <sipaa/x86_64/pit.h>
#include <sipaa/libc/string.h>
#include <sipaa/pmm.h>
#include <sipaa/heap.h>
#include <sipaa/sched.h>
#include <sipaa/logger.h>

uint64_t pid_counter = 0;
ProcessT *current_process = NULL;
ProcessT *process_list = NULL;
ProcessT *idle_process = NULL;

int Scheduler_IdleTask()
{
    while (1)
        ;;

    return 0;
}

void Scheduler_Initialize()
{
    current_process = (ProcessT *)KHeap_Allocate(sizeof(ProcessT));
    current_process->pml4 = Vmm_NewAddressSpace();
    current_process->next = current_process;
    current_process->pid = -1;
    process_list = current_process;

    Scheduler_CreateProcess("idle", Scheduler_IdleTask);
}

void Scheduler_Entry(int (*entry)()) // Todo: this should be handled by the LibC 
{
    // Having a "bootstrapper" is necessary since most of programs are exiting by returning. This handles the situation.
    int result = entry();
    Log(LT_INFO, "Scheduler", "Process %d exited with exit code %d!\n", current_process->pid, result);

    Scheduler_ExitProcess(current_process);
    asm("int $32");
}

ProcessT *Scheduler_CreateProcess(char *name, int (*entry_point)())
{
    ProcessT *new_proc = (ProcessT *)KHeap_Allocate(sizeof(ProcessT));
    CopyStringN(new_proc->name, name, PROCESS_NAME_SIZE);
    new_proc->stack_ptr = (uint64_t)KHeap_Allocate(4096) + 4096;
    new_proc->pml4 = Vmm_NewAddressSpace();
    new_proc->pid = pid_counter;

    new_proc->regs.rip = (uint64_t)Scheduler_Entry;
    new_proc->regs.rdi = (uint64_t)entry_point;
    new_proc->regs.cs = 0x08;
    new_proc->regs.rflags = 0x202;
    new_proc->regs.rsp = new_proc->stack_ptr;
    new_proc->regs.ss = 0x10;

    new_proc->next = process_list->next;
    process_list->next = new_proc;

    pid_counter++;
    
    Log(LT_INFO, "Scheduler", "Created process \"%s\": PID: %d, RIP: %p, AddrSpace: %p\n", new_proc->name, new_proc->pid, new_proc->regs.rip, new_proc->pml4);

    return new_proc;
}

void Scheduler_ExitProcess(ProcessT *proc)
{
    ProcessT *prev_proc = process_list;

    while (prev_proc->next != proc)
    {
        prev_proc = prev_proc->next;
    }

    prev_proc->next = proc->next;

    KHeap_Free((void *)proc->stack_ptr - 4096);
    KHeap_Free(proc);
}

void Scheduler_Schedule(RegistersT *regs)
{
    current_process->regs = *regs;
    current_process = current_process->next;
    *regs = current_process->regs;

    Vmm_SwitchAddressSpaces(current_process->pml4);
    Scheduler_ContextSwitch(regs); 
}