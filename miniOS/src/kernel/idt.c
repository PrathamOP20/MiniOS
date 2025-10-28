// src/kernel/idt.c

#include <stdint.h>
#include "screen.h"

/* ================= Structures ================= */

struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

#define IDT_ENTRIES 256

/* ================= Globals ================= */

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr   idtp;

/* External assembly functions */
extern void idt_load(void);
extern void isr_keyboard(void);
extern void irq_stub(void);

/* ================= Helper ================= */

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low  = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel       = sel;
    idt[num].always0   = 0;
    idt[num].flags     = flags;
}

/* ================= Initialization ================= */


void print_hex(uint32_t val) {
    char hex[9];
    const char* digits = "0123456789ABCDEF";
    for (int i = 0; i < 8; i++) {
        hex[7-i] = digits[val & 0xF];
        val >>= 4;
    }
    hex[8] = '\0';
    print(hex);
}


void idt_init(void) {
    idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idtp.base  = (uint32_t)&idt;
    print("IDT limit: ");
    print_hex(idtp.limit);
    print("IDT_Base:");
    print_hex(idtp.base);

    /* Clear all entries */
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    /* Map IRQ0–IRQ15 (PIC remapped range 0x20–0x2F) to irq_stub */
    for (int i = 0; i < 16; i++) {
        idt_set_gate(0x20 + i, (uint32_t)irq_stub, 0x08, 0x8E);
    }

    /* Overwrite keyboard IRQ (IRQ1 → 0x21) with its actual ISR */
    idt_set_gate(0x21, (uint32_t)isr_keyboard, 0x08, 0x8E);

    /* Load IDT register (uses lidt [idtp]) */
    idt_load();
}
