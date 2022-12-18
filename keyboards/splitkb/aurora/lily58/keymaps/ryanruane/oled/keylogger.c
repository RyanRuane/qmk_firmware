#include QMK_KEYBOARD_H
#include <stdio.h>
#include "action.h"

char keylog_str[24] = {};
char keylogs_str[21] = {};
int keylogs_str_idx = 0;

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ';', '\'', ' ', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
    char name = '?';
    if (keycode < 60) {
        name = code_to_name[keycode];
    }

    // update keylog
    snprintf(
        keylog_str,
        sizeof(keylog_str),
        "%dx%d, k%2d",
        record->event.key.row,
        record->event.key.col,
        keycode
    );

    // update keylogs
    if (keylogs_str_idx == sizeof(keylogs_str) - 1) {
        keylogs_str_idx = 0;
        for (int i = 0; i < sizeof(keylogs_str) - 1; i++) {
            keylogs_str[i] = ' ';
        }
    }

    keylogs_str[keylogs_str_idx++] = ' ';
    keylogs_str[keylogs_str_idx++] = ' ';
    keylogs_str[keylogs_str_idx++] = name;
    keylogs_str[keylogs_str_idx++] = ' ';
    keylogs_str[keylogs_str_idx++] = ' ';
}

const char *read_keylog(void) {
  return keylog_str;
}

const char *read_keylogs(void) {
  return keylogs_str;
}

const uint8_t render_keylogger(uint8_t line) {
    oled_set_cursor(0, line);
    oled_write_ln(read_keylog(), false);
    oled_write_ln(read_keylogs(), false);
    const uint8_t end_line = line + 8;
    return end_line;
}
