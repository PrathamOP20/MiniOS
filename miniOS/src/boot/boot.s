.set MAGIC, 0x1BADB002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

.section .text
.global _start
_start:
    mov $kernel_stack_top, %esp
    call kernel_main
halt:
    hlt
    jmp halt

.section .bss
.space 16384
kernel_stack_top:
