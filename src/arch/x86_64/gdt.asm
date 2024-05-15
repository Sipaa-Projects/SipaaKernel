[bits 64]

global Gdt_Load, Tss_Load

Tss_Load:
    mov ax, 0x30
    ltr ax
    ret

Gdt_Load:
    lgdt [rdi]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp .flush

.flush:
    push 0x08
    push .reload_cs
    retfq

.reload_cs:
    ret