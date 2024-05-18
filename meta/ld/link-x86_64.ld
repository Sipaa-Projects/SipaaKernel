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

    TextStartLD = .;

    .text : {
        *(.text .text.*)
    } :text

    TextEndLD = .;

    . += CONSTANT(MAXPAGESIZE);

    RoDataStartLD = .;

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    RoDataEndLD = .;
    
    . += CONSTANT(MAXPAGESIZE);

    DataStartLD = .;

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

    DataEndLD = .;

    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
    }
}