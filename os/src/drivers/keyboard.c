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

// #define LSHIFT_DOWN 0x2A
// #define LSHIFT_UP 0x0A
// int uppercase_state = 0;

static char key_buffer[256];

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

    // Special keys to handle
    // switch (scancode) {
    //     case 0x1:
    //         kprint("ESC");
    //         break;
	// 	case 0x2A:
	// 		kprint("LShift");
	// 		break;
    //     case 0x36:
    //         kprint("Rshift");
    //         break;
    //     default:
    //         if (scancode <= 0x7f) {
    //             kprint("Unknown key down");
    //         } else if (scancode <= 0x39 + 0x80) {
    //             kprint("key up ");
    //         } else kprint("Unknown key up");
    //         break;
    // }

    // char phys_str[16] = "";
    // hex_to_ascii(scancode, phys_str);
    // kprint(phys_str);
    // kprint("\n");


    // if (scancode == LSHIFT_DOWN) {
    //     uppercase_state = 1;
    // } else if (scancode == LSHIFT_UP) {
    //     uppercase_state = 0;
    // }
    
    if (scancode > SC_MAX) return;
    if (scancode == BACKSPACE) {
        backspace(key_buffer);
        kprint_backspace();
    } else if (scancode == ENTER) {
        kprint("\n");
        user_input(key_buffer); /* kernel-controlled function */
        key_buffer[0] = '\0';
    } else {
        char letter;
        if (0) {
            letter = sc_ascii[(int)scancode];
        } else {
            letter = sc_lower_ascii[(int)scancode];
        }
        /* Remember that kprint only accepts char[] */
        char str[2] = {letter, '\0'};
        append(key_buffer, letter);
        kprint(str);
    }
    UNUSED(regs);
}

void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback); 
}
