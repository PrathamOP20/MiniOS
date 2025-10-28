#include <stdint.h>
#include "io.h"

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2+1)
#define ICW1_INIT    0x10
#define ICW1_ICW4    0x01
#define ICW4_8086    0x01

#include "io.h"

void pic_remap()
{
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC1_DATA, 0x20); // offset PIC1 to 0x20
    outb(PIC2_DATA, 0x28); // offset PIC2 to 0x28
    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    outb(PIC1_DATA, 0xFF); /* mask all IRQs on master PIC */
    outb(PIC2_DATA, 0xFF);
}
