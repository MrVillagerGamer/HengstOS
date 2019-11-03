ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)
SECTIONS
{
    . = 0;
    .text :
    {
        *(.init)
        *(.text)
        *(.text.*)
    }

    .data :
    {
        *(.data)
        *(.data.*)
    }

    .rodata :
    {
        *(.rodata)
        *(.rodata.*)
    }

    .bss :
    {
        *(COMMON)
        *(.bss)
        *(.bss.*)
    }

}