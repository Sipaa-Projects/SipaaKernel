#include <stdint.h>
#include <stdbool.h>
#include "pic.h"

#define MAX_CALLBACKS 10

typedef struct{
    uint64_t cr3;
    
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;  
    uint64_t rdi;
    uint64_t rbp;

    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;

    uint64_t interrupt_number; 
    uint64_t error_code; 
    
    uint64_t rip; 
    uint64_t cs; 
    uint64_t rflags; 
    uint64_t rsp; 
    uint64_t ss;
}__attribute__((packed)) interrupt_frame; 

typedef void (*callback_t)(void);

typedef struct
{
    uint8_t keycode;
    callback_t callback;
} key_choice;

__attribute__((interrupt)) void zero_division_handler(interrupt_frame *frame);
__attribute__((interrupt)) void debug_handler(interrupt_frame *frame);
__attribute__((interrupt)) void gpf_handler(interrupt_frame *frame);
__attribute__((interrupt)) void pf_handler(interrupt_frame *frame);
__attribute__((interrupt)) void non_maskable_interrupt_handler(interrupt_frame *frame);
__attribute__((interrupt)) void breakpoint_handler(interrupt_frame *frame);
__attribute__((interrupt)) void overflow_handler(interrupt_frame *frame);
__attribute__((interrupt)) void bound_range_exceeded_handler(interrupt_frame *frame);
__attribute__((interrupt)) void invalid_opcode_handler(interrupt_frame *frame);
__attribute__((interrupt)) void device_not_available_handler(interrupt_frame *frame);
__attribute__((interrupt)) void double_fault_handler(interrupt_frame *frame);
__attribute__((interrupt)) void invalid_tss_handler(interrupt_frame *frame);
__attribute__((interrupt)) void segment_not_present_handler(interrupt_frame *frame);
__attribute__((interrupt)) void stack_fault_handler(interrupt_frame *frame);
__attribute__((interrupt)) void floating_point_error_handler(interrupt_frame *frame);
__attribute__((interrupt)) void alignment_check_handler(interrupt_frame *frame);
__attribute__((interrupt)) void machine_check_handler(interrupt_frame *frame);
__attribute__((interrupt)) void simd_floating_point_handler(interrupt_frame *frame);
__attribute__((interrupt)) void virtualization_exception_handler(interrupt_frame *frame);
__attribute__((interrupt)) void kb_handler(interrupt_frame *frame);
__attribute__((interrupt)) void ms_handler(interrupt_frame *frame);
__attribute__((interrupt)) void syscall_handler(interrupt_frame *frame);

typedef struct
{
    uint16_t base_low;
    uint16_t cs;
    uint8_t ist;
    uint8_t attributes;
    uint16_t base_mid;
    uint32_t base_high;
    uint32_t rsv0;
} __attribute__((packed)) idt_entry;

typedef struct
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idt_pointer;

void init_idt();
void _panic(char *message, interrupt_frame *frame);

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define panic(message, frame) _panic(__FILE__ ":" TOSTRING(__LINE__) ": " message, frame)