; src/kernel/isr.asm
BITS 32

global idt_load
global isr_keyboard
global irq_stub

extern idtp
extern keyboard_handler_simple   ; safe C handler (no printing inside)
                                 ; you will define this in keyboard.c

section .text

; ------------------------------------------------------------
; Load IDT register
; ------------------------------------------------------------
idt_load:
    lidt [idtp]
    ret


; ------------------------------------------------------------
; Generic IRQ stub (IRQ0–IRQ15)
; Does nothing except acknowledge PIC and return
; ------------------------------------------------------------
irq_stub:
    pusha

    ; No segment changes
    ; No calls
    ; Safe minimal stub

    popa

    mov al, 0x20            ; EOI → Master PIC
    out 0x20, al
    iretd


; ------------------------------------------------------------
; Keyboard IRQ handler (IRQ1 → 0x21)
; Safe version:
;  - No DS/ES loads
;  - No reloading segments
;  - No calling print/putchar
;  - Only reads port 0x60 and pushes into buffer (via C handler)
; ------------------------------------------------------------
isr_keyboard:
    pusha

    call keyboard_handler_simple

    popa

    mov al, 0x20            ; EOI → Master PIC
    out 0x20, al
    iretd
