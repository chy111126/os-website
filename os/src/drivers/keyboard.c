#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"
#include <stdint.h>

#define BACKSPACE 0x0E
#define ENTER 0x1C

// Arrow keys, with extended code 0xE0
#define ARROW_UP 0x48
#define ARROW_DOWN 0x50
#define ARROW_LEFT 0x4B
#define ARROW_RIGHT 0x4D

// A ring-buffer for keystrokes
#define MAX_BUFFER 16
static int MAX_KEY_BUFFER = MAX_BUFFER;
static char key_buffer[MAX_BUFFER];
static int key_buffer_i = 0;

#define SC_MAX 57
const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_lower_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y', 
        'u', 'i', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
        'h', 'j', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
        'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};
const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

static void keyboard_callback(registers_t *regs) {
    /* The PIC leaves us the scancode in port 0x60 */
    uint8_t scancode = port_byte_in(0x60);

    // Skip unsupported keystrokes
    // A small hack to accept arrow keys
    if (scancode > SC_MAX && scancode != ARROW_UP && scancode != ARROW_DOWN && scancode != ARROW_LEFT && scancode != ARROW_RIGHT) return;

    // Translate letter to ASCII
    char letter;
    letter = sc_ascii[(int)scancode];

    // TODO: Make it less duplicated and confusing
    if (scancode == BACKSPACE || scancode == ENTER || scancode == ARROW_UP || scancode == ARROW_DOWN || scancode == ARROW_LEFT || scancode == ARROW_RIGHT) {
        key_buffer[key_buffer_i++] = scancode;

        if (key_buffer_i == MAX_KEY_BUFFER) {
            key_buffer_i = 0;
        }
    } else if (letter - '?') {
        // Skip non-typable characters
        // Fill the keystroke buffer
        // key_buffer_i would wrap around after reaching its max buffer
        key_buffer[key_buffer_i++] = letter;

        if (key_buffer_i == MAX_KEY_BUFFER) {
            key_buffer_i = 0;
        }
    }

    // if (scancode > SC_MAX) return;
    // if (scancode == BACKSPACE) {
    //     backspace(key_buffer);
    //     kprint_backspace();
    // } else if (scancode == ENTER) {
    //     kprint("\n");
    //     user_input(key_buffer); /* kernel-controlled function */
    //     key_buffer[0] = '\0';
    // } else {
    //     char letter;
    //     letter = sc_ascii[(int)scancode];
    //     // Skip non-typable characters
    //     if (letter - '?') {
    //         /* Remember that kprint only accepts char[] */
    //         char str[2] = {letter, '\0'};
    //         append(key_buffer, letter);
    //         kprint(str);
    //     }
    // }
    UNUSED(regs);
}

void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback); 

   // Clear keyboard buffer
//    uint32_t i = 0;
//    while (i < 256) {
//        key_buffer[i++] = '\0';
//    }
}

char* get_keyboard_buffer() {
    return key_buffer;
}

int get_keyboard_buffer_i() {
    // For keyboard buffer in kernel, returns i for next available input slot
    return key_buffer_i;
}

int get_max_keyboard_buffer() {
    return MAX_KEY_BUFFER;
}

char get_last_keystroke() {
    int key_buffer_i = get_keyboard_buffer_i();
    if (key_buffer_i == 0) {
        key_buffer_i = MAX_KEY_BUFFER;
    }
    return key_buffer[key_buffer_i-1];
}