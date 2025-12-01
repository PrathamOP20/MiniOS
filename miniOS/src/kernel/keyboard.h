// src/kernel/keyboard.h
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

extern volatile char key_buffer[256];
extern volatile int key_buffer_index;

void keyboard_handler_simple(void);
void keyboard_init(void);

#endif
