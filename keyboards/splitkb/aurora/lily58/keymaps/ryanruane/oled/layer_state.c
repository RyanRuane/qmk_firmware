#include QMK_KEYBOARD_H
#include <stdio.h>

#define L_MAIN 0
#define L_LOWER (1 << 3)
#define L_RAISE (1 << 4)
#define L_ADJUST (1 << 5)
#define L_ADJUST_TRI (L_ADJUST | L_RAISE | L_LOWER)

char base_state_str[24];
char layer_state_str[24];

void set_label_dvorak(void) {
    snprintf(base_state_str, sizeof(base_state_str), "DVRAK=====");
}

void set_label_qwerty(void) {
    snprintf(base_state_str, sizeof(base_state_str), "QWRTY=====");
}

void set_label_plover(void) {
    snprintf(base_state_str, sizeof(base_state_str), "PLOVR=====");
}

const char *read_layer_state(void) {
    switch (layer_state) {
        case L_MAIN:
            return base_state_str;
        case L_RAISE:
            snprintf(layer_state_str, sizeof(layer_state_str), "RAISE=====");
            break;
        case L_LOWER:
            snprintf(layer_state_str, sizeof(layer_state_str), "LOWER=====");
            break;
        case L_ADJUST:
        case L_ADJUST_TRI:
            snprintf(layer_state_str, sizeof(layer_state_str), "ADJST=====");
            break;
        default:
            snprintf(layer_state_str, sizeof(layer_state_str), "UNDEF%u", layer_state);
    }

    return layer_state_str;
}

const uint8_t render_lily_layer_state(uint8_t line) {
    oled_set_cursor(0, line);
    oled_write(read_layer_state(), false);
    const uint8_t end_line = line + 2;
    return end_line;
}
