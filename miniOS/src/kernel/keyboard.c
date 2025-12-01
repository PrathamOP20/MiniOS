// src/kernel/keyboard.c
#include <stdint.h>
#include "io.h"
#include "keyboard.h"

/* Shared buffer accessed by ISR and main loop */
volatile char key_buffer[256];
volatile int key_buffer_index = 0;

/* Scancode → ASCII map */
static const char scancode_map[128] = {
    0, 27,'1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',
};

/* ============================================================
   Minimal, safe ISR handler called from isr_keyboard in ASM
   - No printing
   - No DS/ES changes required
   - No unsafe C library calls
   ============================================================ */
void keyboard_handler_simple(void) {
    uint8_t sc = inb(0x60);

    if (sc & 0x80) return;      // ignore key releases
    if (sc >= 128) return;      // invalid scancode

    char c = scancode_map[sc];
    if (!c) return;             // unmapped

    if (key_buffer_index < 255)
        key_buffer[key_buffer_index++] = c;
}

/* ============================================================
   Unmask IRQ1 (keyboard)
   ============================================================ */
void keyboard_init(void) {
    uint8_t mask = inb(0x21);
    mask &= ~(1 << 1);      // enable IRQ1
    outb(0x21, mask);
}
