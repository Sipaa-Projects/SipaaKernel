[bits 64]

global load_gdt, load_tss

load_tss:
    mov ax, 0x30
    ltr ax
    ret

load_gdt:
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
