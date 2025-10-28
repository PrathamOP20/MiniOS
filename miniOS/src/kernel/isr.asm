; src/kernel/isr.asm
BITS 32

global idt_load
global isr_keyboard
global irq_stub

extern idtp
extern keyboard_handler

section .text

; Load the IDT register with address stored in 'idtp' (defined in C)
idt_load:
    lidt [idtp]
    ret

; Generic IRQ stub (for unused IRQs)
irq_stub:
    pusha
    push ds
    push es

    mov ax, 0x10
    mov ds, ax
    mov es, ax

    pop es
    pop ds
    popa

    mov al, 0x20
    out 0x20, al
    iretd

; Keyboard IRQ handler (IRQ1 → 0x21)
isr_keyboard:
    pusha
    push ds
    push es

    mov ax, 0x10
    mov ds, ax
    mov es, ax

    call keyboard_handler

    pop es
    pop ds
    popa

    mov al, 0x20
    out 0x20, al
    iretd
