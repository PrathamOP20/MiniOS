// src/kernel/keyboard.c
#include <stdint.h>
#include "screen.h"
#include "io.h"
#include "keyboard.h"

/* make these volatile because they're shared between ISR and main loop */
volatile char key_buffer[256];
volatile int key_buffer_index = 0;
void keyboard_handler(void);


char scancode_map[128] = {
    0, 27,'1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',
};

/* Interrupt-safe keyboard handler:
 * - Run in interrupt context (called by assembly ISR)
 * - Must not call non-reentrant functions (no putchar/print)
 * - Push character into circular/linear buffer for main loop to consume
 */
// void keyboard_handler(void) {
//     uint8_t scancode = inb(0x60);
//     if (scancode & 0x80) return;  // key release
//     if (scancode >= sizeof(scancode_map)) return;
//     char c = scancode_map[scancode];
//     if (!c) return;

//     int idx = key_buffer_index;
//     if (idx < 255) {
//         key_buffer[idx] = c;
//         key_buffer_index = idx + 1;
//     }
// }

#include <stdint.h>
#include "io.h"

volatile int irq_test_count = 0;

void __attribute__((cdecl)) keyboard_handler(void) {
    // uint8_t scancode = inb(0x60);
    // if (scancode & 0x80) return;
    // if (scancode >= 128) return;
    // char c = scancode_map[scancode];
    // if (!c) return;
    // int idx = key_buffer_index;
    // if (idx < 255) {
    //     key_buffer[idx] = c;
    //     key_buffer_index = idx + 1;
    // }
    (void)inb(0x60);
}



/* Enable keyboard IRQ (IRQ1) on master PIC */
void keyboard_init() {
    uint8_t mask = inb(0x21);
    mask &= ~(1 << 1); // clear bit 1 (IRQ1)
    outb(0x21, mask);
}
