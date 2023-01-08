#include QMK_KEYBOARD_H
#include <stdio.h>
#include "action.h"

#define KEYLOG_WIDTH 5
#define KEYLOG_QUEUE_CAPACITY 6
#define KEYLOG_CLEAR_DELAY_REPETITIONS 4

char key_info_str[24] = "TOUCHME!";
char keylog_queue[KEYLOG_QUEUE_CAPACITY] =  { [0 ... KEYLOG_QUEUE_CAPACITY-1] = ' ' };
uint8_t keylog_queue_first_elem_position = KEYLOG_QUEUE_CAPACITY;
uint8_t keylog_queue_end_idx = 0;
uint8_t keylog_clear_delay_idx = 0;

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ';', '\'', ' ', ',', '.', '/', ' ', ' ', ' '
};

static char get_key_name(uint16_t keycode) {
    char name = '?';
    if (keycode < 60) {
        name = code_to_name[keycode];
    }
    return name;
}

static void update_keylog_queue(uint16_t keycode) {
    keylog_queue[keylog_queue_end_idx] = get_key_name(keycode);
    keylog_queue_end_idx = (keylog_queue_end_idx + 1) % KEYLOG_QUEUE_CAPACITY;
}

void add_to_keylog_queue(uint16_t keycode) {
    update_keylog_queue(keycode);
    keylog_queue_first_elem_position = 0;
}

void del_from_keylog_queue(void) {
    if (keylog_queue_first_elem_position < KEYLOG_QUEUE_CAPACITY) {
        update_keylog_queue(0);
        keylog_queue_first_elem_position++;
    }
}

void slow_clear_keylog_queue(void) {
    int8_t keylog_clear_count = keylog_clear_delay_idx / (KEYLOG_CLEAR_DELAY_REPETITIONS - 1);
    for (int8_t i = 0; i < keylog_clear_count; i++) {
        del_from_keylog_queue();
    }
    keylog_clear_delay_idx = (keylog_clear_delay_idx + 1) % KEYLOG_CLEAR_DELAY_REPETITIONS;
}

const uint8_t keylog_queue_empty(void) {
    return keylog_queue_first_elem_position == KEYLOG_QUEUE_CAPACITY;
}

const uint8_t render_keylog_queue(uint8_t line) {
    uint8_t curr_row = line;
    uint8_t curr_col = 2;
    // Print queue backwards from end
    for (int8_t i = keylog_queue_end_idx; i > 0; i--) {
        const uint8_t curr_char_idx = i-1;
        oled_set_cursor(curr_col, curr_row);
        oled_write_char(keylog_queue[curr_char_idx], false);
        curr_row++;
    }
    // Print rest of queue backwards
    for (int8_t i = KEYLOG_QUEUE_CAPACITY; i > keylog_queue_end_idx; i--) {
        const uint8_t curr_char_idx = i-1;
        oled_set_cursor(curr_col, curr_row);
        oled_write_char(keylog_queue[curr_char_idx], false);
        curr_row++;
    }
    return line + KEYLOG_QUEUE_CAPACITY;
}

void update_key_info(uint16_t keycode, keyrecord_t *record) {
    // update keylog
    snprintf(
        key_info_str,
        sizeof(key_info_str),
        "%dx%d, k%2d",
        record->event.key.row,
        record->event.key.col,
        keycode
    );
}

const uint8_t render_key_info(uint8_t line) {
    oled_set_cursor(0, line);
    oled_write_ln(key_info_str, false);
    return line + 2;
}
