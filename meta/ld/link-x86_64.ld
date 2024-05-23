OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

ENTRY(SKEntry)

PHDRS
{
    text    PT_LOAD    FLAGS((1 << 0) | (1 << 2)) ;
    rodata  PT_LOAD    FLAGS((1 << 2)) ;
    data    PT_LOAD    FLAGS((1 << 1) | (1 << 2)) ;
    dynamic PT_DYNAMIC FLAGS((1 << 1) | (1 << 2)) ;
}

SECTIONS
{
    . = 0xffffffff80000000;

    text_start_ld = .;

    .text : {
        *(.text .text.*)
    } :text

    text_end_ld = .;

    . += CONSTANT(MAXPAGESIZE);

    rodata_start_ld = .;

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    rodata_end_ld = .;
    
    . += CONSTANT(MAXPAGESIZE);

    data_start_ld = .;

    .data : {
        *(.data .data.*)
    } :data

    .dynamic : {
        *(.dynamic)
    } :data :dynamic

    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    } :data

    data_end_ld = .;

    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
    }
}