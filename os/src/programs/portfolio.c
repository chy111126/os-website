#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../libc/string.h"
#include "../kernel/kernel.h"
#include "portfolio.h"

#define ENTER 0x1C
#define BOTTOM_ROW 23

// Arrow keys, with extended code 0xE0
#define ARROW_UP 0x48
#define ARROW_DOWN 0x50
#define ARROW_LEFT 0x4B
#define ARROW_RIGHT 0x4D

void portfolio_main() {

    // Static data
    const int links_num = 4;
    int links_cols[links_num];
    links_cols[0] = links_cols[2] = 12;
    links_cols[1] = links_cols[3] = links_cols[0] + 40;
    int links_rows[links_num];
    links_rows[0] = links_rows[1] = 12;
    links_rows[2] = links_rows[3] = links_rows[0] + 2;
    const char* links_str[links_num];
    links_str[0] = "Education";
    links_str[1] = "Work Experience";
    links_str[2] = "Contacts";
    links_str[3] = "Exit";

    // Program status
    int exit_status = 1; // 0 - Program exit, 1 - Program stays in loop
    int cursor_status = 1; // 0 - Enter pressed, 1 - Cursor selecting
    int cursor_link_index = 0;
    // Location: From top-left
    const int cursor_location_x_max = 2;
    const int cursor_location_y_max = 2;
    int cursor_location_x = 0;
    int cursor_location_y = 0;

    // Keyboard buffer, unused for keystroke mode
    char* command[2];
    command[0] = '\0';

    // Input loop
    int program_status = 1;
    while(program_status) {
        // Print main page
        clear_screen();
        kprint_at("Welcome to Tom Chau's homepage!", 2, 1);
        kprint_at("Last updated: Feb 2022", 2, 3);

        for(int i = 0; i < links_num; i++) {
            // Print the choices
            kprint_at(links_str[i], links_cols[i], links_rows[i]);

            // Print the selector box before the choices
            kprint_at("[ ]", links_cols[i] - 4, links_rows[i]);
        }

        // Decide next action
        while(cursor_status) {
            // Print the cursor
            kprint_at("[", links_cols[cursor_link_index] - 4, links_rows[cursor_link_index]);

            char curr_char = wait_for_keyboard_input(1, command);

            if (curr_char == ENTER) {
                cursor_status = 0;
            } else if (curr_char == ARROW_UP) {
                cursor_location_y -= 1;
            } else if (curr_char == ARROW_DOWN) {
                cursor_location_y += 1;
            } else if (curr_char == ARROW_LEFT) {
                cursor_location_x -= 1;
            } else if (curr_char == ARROW_RIGHT) {
                cursor_location_x += 1;
            } else {
                // Do nothing
            }

            // Process cursor_link_index
            if (cursor_location_x < 0) {
                cursor_location_x = 0;
            }
            if (cursor_location_x > cursor_location_x_max-1) {
                cursor_location_x = cursor_location_x_max-1;
            }
            if (cursor_location_y < 0) {
                cursor_location_y = 0;
            }
            if (cursor_location_y > cursor_location_y_max-1) {
                cursor_location_y = cursor_location_y_max-1;
            }
            cursor_link_index = cursor_location_y * cursor_location_y_max + cursor_location_x;
        }

        // TODO: Logic to jump to different pages
        if (cursor_link_index == 0) {
            portfolio_education_page();
        } else if (cursor_link_index == 1) {
            portfolio_work_experience_page();
        } else if (cursor_link_index == 2) {
            portfolio_contacts_page();
        } else if (cursor_link_index == 3) {
            program_status = 0;
        }
        
        // Reset for next selection
        cursor_status = 1;
    }

    // Exit program
    return 0;
}

void portfolio_education_page() {

    // Program status
    int cursor_status = 1; // 0 - Enter pressed, 1 - Cursor selecting

    // Keyboard buffer, unused for keystroke mode
    char* command[2];
    command[0] = '\0';

    // Print educatio page
    clear_screen();
    kprint_at("Education", 2, 1);

    kprint_at("[ ] Back", 2, BOTTOM_ROW);
    // Decide next action
    while(cursor_status) {
        // Print the cursor
        kprint_at("[", 2, BOTTOM_ROW);

        char curr_char = wait_for_keyboard_input(1, command);

        if (curr_char == ENTER) {
            cursor_status = 0;
        } else {
            // Do nothing
        }

    }

    // Exit page
    return;
}

void portfolio_work_experience_page() {

    // Program status
    int cursor_status = 1; // 0 - Enter pressed, 1 - Cursor selecting

    // Keyboard buffer, unused for keystroke mode
    char* command[2];
    command[0] = '\0';

    // Print educatio page
    clear_screen();
    kprint_at("Work Experience", 2, 1);

    kprint_at("[ ] Back", 2, BOTTOM_ROW);
    // Decide next action
    while(cursor_status) {
        // Print the cursor
        kprint_at("[", 2, BOTTOM_ROW);

        char curr_char = wait_for_keyboard_input(1, command);

        if (curr_char == ENTER) {
            cursor_status = 0;
        } else {
            // Do nothing
        }

    }

    // Exit page
    return;
}

void portfolio_contacts_page() {

    // Program status
    int cursor_status = 1; // 0 - Enter pressed, 1 - Cursor selecting

    // Keyboard buffer, unused for keystroke mode
    char* command[2];
    command[0] = '\0';

    // Print educatio page
    clear_screen();
    kprint_at("Contacts", 2, 1);

    kprint_at("[ ] Back", 2, BOTTOM_ROW);
    // Decide next action
    while(cursor_status) {
        // Print the cursor
        kprint_at("[", 2, BOTTOM_ROW);

        char curr_char = wait_for_keyboard_input(1, command);

        if (curr_char == ENTER) {
            cursor_status = 0;
        } else {
            // Do nothing
        }

    }

    // Exit page
    return;
}