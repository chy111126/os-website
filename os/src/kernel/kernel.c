#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../drivers/serial_port.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include <stdint.h>

#define BACKSPACE 0x0E
#define ENTER 0x1C

#define INPUT_MAX_CHARS 77
// int INPUT_BUFFER_MAX_CHARS = INPUT_MAX_CHARS;

void kernel_main() {
    isr_install();
    irq_install();


    kprint("Type something, it will go through the kernel\n"
        "Type END to halt the CPU or PAGE to request a kmalloc()\n> ");

    char* command[INPUT_MAX_CHARS];
    command[0] = '\0';
    while(1) {
        // Program routines goes here ...
        user_input(command);

        // Get input from keyboard interrupts
        wait_for_keyboard_input(command);
    }
}

void wait_for_keyboard_input(out_buffer) {
    //TODO: Avoid using strlen and counting the input_buffer strlen ourselves

    // Kernel function keeping its own buffer
    char input_buffer[INPUT_MAX_CHARS];
    // Clear input_buffer
    uint32_t input_buffer_i = 0;
    while (input_buffer_i < INPUT_MAX_CHARS) {
        input_buffer[input_buffer_i++] = '\0';
    }
    input_buffer_i = 0;
    char* curr_input_ptr = input_buffer;

    // Info from keyboard driver
    char* keyboard_buffer = get_keyboard_buffer();
    char* curr_buf_ptr = keyboard_buffer;
    int key_buffer_start_i = get_keyboard_buffer_i();
    int key_buffer_max = get_max_keyboard_buffer();

    // Continuously getting and flushing keyboard driver's buffer
    while (1) {

        int key_buffer_i = get_keyboard_buffer_i();
        
        // Copy from keyboard buffer to function buffer (aligned with ring buffer index)
        while (key_buffer_start_i != key_buffer_i) {
            // Get character
            char curr_char = keyboard_buffer[key_buffer_start_i];
            
            // Write to terminal
            write_serial(curr_char);
            
            // Check special characters
            if (curr_char == BACKSPACE) {
                // Backspace: remove one character from input_buffer
                // Only if there is SOMETHING in the input_buffer
                if (strlen(input_buffer) > 0) {
                    backspace(input_buffer);
                    kprint_backspace();
                }
            } else if (curr_char == ENTER) {
                // TODO: Pressing ENTER should return the function back to kernel_main
                kprintln("");
                //kprint("> ");
                // user_input(input_buffer); /* kernel-controlled function */
                // input_buffer[0] = '\0';
                strcpy(input_buffer, out_buffer, INPUT_MAX_CHARS);
                return;
            } else if (strlen(input_buffer) == INPUT_MAX_CHARS-1) {
                // Not accepting new inputs if it exceeded input_buffer maximum
                // -1 for null terminator safety
            } else {
                append(input_buffer, curr_char);
                // Printing to screen
                char str[2] = {curr_char, '\0'};
                kprint(str);
            }

            // Ring-buffer alignment logic
            input_buffer_i++;
            key_buffer_start_i++;
            if (key_buffer_start_i == key_buffer_max) {
                key_buffer_start_i = 0;
            }
        }
    }
}

void user_input(char *input) {
    // User-input should only handles short-burst task, for subroutines, please control it with kernel_main()

    if (strcmp(input, "END") == 0) {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("cli");
        asm volatile("hlt");
    } else if (strcmp(input, "PAGE") == 0) {
        /* Lesson 22: Code to test kmalloc, the rest is unchanged */
        uint32_t phys_addr;
        uint32_t page = kmalloc(1000, 1, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n");
    } else if (strcmp(input, "TEST") == 0) {
        kprint("Your first command!\n");
    } else if (strcmp(input, "GETTICK") == 0) {
        uint32_t tick = get_tick();
        char tick_str[16] = "";
        int_to_ascii(tick, tick_str);
        kprint_animated("Current tick is at: ");
        kprintln_animated(tick_str);
    } else if (strcmp(input, "CLS") == 0) {
        clear_screen();
    } else if (strcmp(input, '\0') == 0) {
    } else {
        kprint("You said: ");
        kprint(input);
    }
    kprint("\n> ");
}
