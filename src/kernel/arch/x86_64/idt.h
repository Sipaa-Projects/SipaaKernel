#include <stdint.h>
#include <stdbool.h>
#include "pic.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define panic(message, frame) _panic(__FILE__ ":" TOSTRING(__LINE__) ": " message, frame)
#define MAX_CALLBACKS 10

struct interrupt_frame
{
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
    uint64_t int_num, error_code;
    uint64_t rip, cs, rflags, rsp, ss;
};

typedef void (*callback_t)(void);

typedef struct
{
    uint8_t keycode;
    callback_t callback;
} key_choice;

__attribute__((interrupt)) void zero_division_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void debug_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void gpf_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void pf_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void non_maskable_interrupt_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void breakpoint_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void overflow_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void bound_range_exceeded_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void invalid_opcode_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void device_not_available_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void double_fault_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void invalid_tss_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void segment_not_present_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void stack_fault_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void floating_point_error_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void alignment_check_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void machine_check_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void simd_floating_point_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void virtualization_exception_handler(struct interrupt_frame *frame);
__attribute__((interrupt)) void kb_handler(struct interrupt_frame *frame);

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