#include <stdint.h>
#include "screen.h"

#define VIDEO_MEMORY 0xB8000
#define WHITE_ON_BLACK 0x0F

static uint16_t* const video_memory = (uint16_t*) VIDEO_MEMORY;
int cursor_pos = 0;

extern int cursor_pos;

void putchar(char c) {
    volatile uint16_t* vga = (uint16_t*)0xB8000;

    if (c == '\n') {
        cursor_pos += 80 - (cursor_pos % 80);
    } else if (c == '\b') {
        if (cursor_pos > 0) cursor_pos--;
        vga[cursor_pos] = 0x0F00; // erase
    } else {
        vga[cursor_pos] = (0x0F << 8) | c;
        cursor_pos++;
    }

    update_cursor(cursor_pos);
}


void print(const char* str) {
    for (int i = 0; str[i]; i++) {
        putchar(str[i]);
    }
}
#include "io.h"

void update_cursor(int pos) {
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}
