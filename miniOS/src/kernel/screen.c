#include <stdint.h>
#include "screen.h"

#define VIDEO_MEMORY 0xB8000
#define WHITE_ON_BLACK 0x0F

static uint16_t* const video_memory = (uint16_t*) VIDEO_MEMORY;
int cursor_pos = 0;

void putchar(char c) {
    if (c == '\n') {
        cursor_pos += 80 - (cursor_pos % 80);
    }
    else if(c=='\b')
    {
        cursor_pos--;
        video_memory[cursor_pos]=(0x0F << 8) | 0x20;
    }
     else {
        video_memory[cursor_pos++] = (WHITE_ON_BLACK << 8) | c;
        // print(video_memory[cursor_pos-1]);
    }
}

void print(const char* str) {
    for (int i = 0; str[i]; i++) {
        putchar(str[i]);
    }
}
