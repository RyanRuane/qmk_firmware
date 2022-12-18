
#include QMK_KEYBOARD_H
#include <stdio.h>

#define L_BASE 0
#define L_LOWER (1 << 1)
#define L_RAISE (1 << 2)
#define L_ADJUST (1 << 3)
#define L_ADJUST_TRI (L_ADJUST | L_RAISE | L_LOWER)

char layer_state_str[24];

const char *read_layer_state(void) {
    // A 128x32 OLED rotated 90 degrees is 5 characters wide and 16 characters tall
    switch (layer_state) {
        case L_BASE:
            snprintf(layer_state_str, sizeof(layer_state_str), "MAIN =====");
            break;
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

const uint8_t render_clap_layer_state(uint8_t line) {
    oled_set_cursor(0, line);
    oled_write(read_layer_state(), false);
    const uint8_t end_line = line + 2;
    return end_line;
}
