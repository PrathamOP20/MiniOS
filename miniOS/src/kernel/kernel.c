#include "screen.h"
#include "keyboard.h"
#include "idt.h"
#include "pic.h"
#include <stdint.h>

// Externs from interrupt/keyboard code
extern int cursor_pos;
extern volatile char key_buffer[256];
extern volatile int key_buffer_index;

int strcmp(const char *a, const char *b)
{
    while (*a && *a == *b)
    {
        a++;
        b++;
    }
    return *a - *b;
}

int strncmp(const char *a, const char *b, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (a[i] != b[i] || a[i] == '\0')
            return a[i] - b[i];
    }
    return 0;
}

void clear_screen()
{
    for (int i = 0; i < 80 * 25; i++)
        ((uint16_t *)0xB8000)[i] = 0x0F00;
}

void print_welcome()
{
    print("miniOS shell ready!\npratham/MiniOS> ");
}

void kernel_main()
{
    // ------------------------
    // Initialize hardware
    // ------------------------
    clear_screen();
    
    pic_remap();   // Remap PIC IRQs
    idt_init();    // Load IDT
    keyboard_init(); // Initialize keyboard handler
    asm volatile("sti"); 
    print_welcome();

    char buffer[256];
    int index = 0;

    while (1)
    {
        // Process characters received via interrupt
        if (key_buffer_index > 0)
        {
            char c = key_buffer[--key_buffer_index];  // get key from buffer

            if (!c) continue;
            if (index >= 256) continue;

            if (c == '\n')
            {
                buffer[index] = '\0';

                if (index > 0)
                {
                    if (strcmp(buffer, "help") == 0)
                    {
                        print("\nCommands: help, echo, clear\n");
                    }
                    else if (strncmp(buffer, "echo ", 5) == 0)
                    {
                        print("\n");
                        print(buffer + 5);
                    }
                    else if (strcmp(buffer, "clear") == 0)
                    {
                        clear_screen();
                        index = 0;
                        for (int i = 0; i < 256; i++) buffer[i] = '\0';
                        cursor_pos = 0;
                        print_welcome();
                        continue;
                    }
                    else
                    {
                        print("\nUnknown command\n");
                    }
                }

                index = 0;
                print("\npratham/MiniOS> ");
            }
            else if (c == '\b')
            {
                if (index > 0)
                {
                    index--;
                    buffer[index] = '\0';
                    putchar('\b');
                }
            }
            else
            {
                buffer[index++] = c;
                putchar(c);
            }
        }
    }
}
