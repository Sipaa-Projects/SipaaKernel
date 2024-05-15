[extern idt]
idt_descriptor:
  dw 4095
  dq idt

  %macro PUSHALL 0
      push rax
      push rcx
      push rdx
      push rbx
      push rbp
      push rsi
      push rdi
      push r8
      push r9
      push r10
      push r11
      push r12
      push r13
      push r14
      push r15
  %endmacro

  %macro POPALL 0
      pop r15
      pop r14
      pop r13
      pop r12
      pop r11
      pop r10
      pop r9
      pop r8
      pop rdi
      pop rsi
      pop rbp
      pop rbx
      pop rdx
      pop rcx
      pop rax
  %endmacro

[extern general_interrupt_handler]

%macro MAKE_ISR 1
global isr%1
isr%1:
    push 0
    push %1
    PUSHALL
    mov rdi, rsp
    call general_interrupt_handler
    POPALL
    add rsp, 16
    iretq
%endmacro

MAKE_ISR 0
MAKE_ISR 1
MAKE_ISR 2
MAKE_ISR 3
MAKE_ISR 4
MAKE_ISR 5
MAKE_ISR 6
MAKE_ISR 7
MAKE_ISR 8
MAKE_ISR 9
MAKE_ISR 10
MAKE_ISR 11
MAKE_ISR 12
MAKE_ISR 13
MAKE_ISR 14
MAKE_ISR 16
MAKE_ISR 17
MAKE_ISR 18
MAKE_ISR 19
MAKE_ISR 20
MAKE_ISR 21
MAKE_ISR 22
MAKE_ISR 23
MAKE_ISR 24
MAKE_ISR 25
MAKE_ISR 26
MAKE_ISR 27
MAKE_ISR 28
MAKE_ISR 29
MAKE_ISR 30
MAKE_ISR 31
MAKE_ISR 32
MAKE_ISR 33

global CR2_Read

CR2_Read:
  mov rax,cr2
  ret

global Idt_Load

Idt_Load:
  lidt[idt_descriptor]
  ret