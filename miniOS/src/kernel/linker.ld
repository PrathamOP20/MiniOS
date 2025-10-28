ENTRY(_start)

SECTIONS
{
    . = 1M; /* Load the kernel at 1 MB */

    .text : ALIGN(4K)
    {
        *(.multiboot)
        *(.text*)
    } :text

    .rodata : ALIGN(4K)
    {
        *(.rodata*)
    } :rodata

    .data : ALIGN(4K)
    {
        *(.data*)
    } :data

    .bss : ALIGN(4K)
    {
        *(COMMON)
        *(.bss*)
    } :data
}

/* Program headers (set memory flags) */
PHDRS
{
    text   PT_LOAD FLAGS(5); /* R + X */
    rodata PT_LOAD FLAGS(4); /* R */
    data   PT_LOAD FLAGS(6); /* R + W */
}
