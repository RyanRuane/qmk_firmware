#include QMK_KEYBOARD_H
#include <stdio.h>

#define IDLE_FRAMES 1
#define IDLE_SPEED 5
#define CLAP_FRAMES 2
#define ANIM_SIZE 128

uint8_t current_idle_frame = 0;
uint8_t current_clap_frame = 0;

static const char PROGMEM raw_logo_idle[IDLE_FRAMES][ANIM_SIZE] = {
    {
        255, 255, 255, 255, 255, 255, 255, 63, 129, 253, 249, 243, 231, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 231, 243, 249, 253, 129, 63, 127, 255, 255, 255, 255, 255, 255, 255, 255, 1, 252, 255, 255, 255, 255, 255, 251, 251, 251, 255, 255, 255, 239, 231, 239, 255, 255, 251, 251, 251, 255, 255, 252, 1, 255, 255, 127, 127, 127, 127, 127, 126, 124, 121, 115, 7, 239, 191, 191, 191, 191, 191, 63, 127, 255, 255, 127, 63, 191, 191, 191, 191, 191, 3, 248, 254, 255, 128, 62, 127, 127, 127, 127, 127, 127, 7, 112, 127, 127, 127, 119, 119, 119, 103, 15, 62, 127, 127, 62, 15, 103, 119, 119, 127, 127, 112, 7, 255, 255,
    },
};

static const char PROGMEM raw_logo_clap[CLAP_FRAMES][ANIM_SIZE] = {
    {
        255, 255, 247, 227, 247, 255, 255, 63, 129, 253, 249, 243, 231, 239, 239, 239, 237, 232, 237, 239, 239, 239, 239, 231, 243, 249, 253, 129, 63, 111, 199, 239, 255, 251, 113, 59, 127, 255, 1, 252, 255, 255, 255, 255, 255, 255, 251, 255, 255, 255, 255, 239, 231, 239, 255, 255, 255, 251, 255, 255, 255, 252, 1, 255, 7, 243, 251, 250, 243, 231, 206, 156, 57, 115, 7, 239, 191, 131, 249, 253, 249, 3, 255, 255, 255, 255, 3, 249, 253, 249, 131, 191, 3, 248, 254, 255, 252, 249, 243, 231, 207, 159, 63, 127, 7, 112, 127, 127, 127, 119, 119, 119, 103, 14, 63, 127, 127, 63, 14, 103, 119, 119, 127, 127, 112, 7, 255, 255,
    },
    {
        255, 255, 235, 247, 235, 255, 255, 63, 129, 253, 249, 243, 231, 239, 239, 239, 234, 237, 234, 239, 239, 239, 239, 231, 243, 249, 253, 129, 63, 87, 239, 215, 255, 245, 187, 117, 191, 255, 1, 252, 255, 255, 255, 255, 255, 255, 251, 255, 255, 255, 255, 239, 231, 239, 255, 255, 255, 251, 255, 255, 255, 252, 1, 255, 255, 127, 126, 127, 126, 127, 126, 124, 121, 115, 7, 239, 255, 255, 207, 231, 243, 121, 60, 142, 224, 142, 60, 121, 243, 231, 255, 255, 3, 248, 254, 255, 128, 62, 127, 127, 127, 127, 127, 127, 7, 112, 127, 127, 127, 119, 119, 119, 103, 14, 63, 127, 127, 63, 15, 102, 119, 119, 127, 127, 112, 7, 255, 255,
    },
};

const uint8_t render_clap_logo(uint8_t line) {
    oled_set_cursor(0, line);
    if (get_current_wpm() <= IDLE_SPEED) {
        current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
        oled_write_raw_P(raw_logo_idle[current_idle_frame], ANIM_SIZE);
    } else {
        current_clap_frame = (current_clap_frame + 1) % CLAP_FRAMES;
        oled_write_raw_P(raw_logo_clap[current_clap_frame], ANIM_SIZE);
    }
    const uint8_t end_line = line + 5;
    oled_set_cursor(0, end_line);
    return end_line;
}
