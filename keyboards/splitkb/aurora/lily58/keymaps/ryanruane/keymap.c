/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "keymap_steno.h"

///////////////////////
/* Tap Dance Support */
///////////////////////

// Define a type for as many tap dance states as you need
typedef enum { TD_NONE, TD_UNKNOWN, TD_SINGLE_TAP, TD_SINGLE_HOLD, TD_DOUBLE_TAP } td_state_t;

typedef struct {
    bool       is_press_action;
    td_state_t state;
} td_tap_t;

// Our custom tap dance key; add any other tap dance keys to this enum
enum {
    LOW_LAYR,
    HIGH_LAYR,
    CAPS_SHIFT,
};

// Declare the functions to be used with your tap dance key(s)

// Function associated with all tap dances
td_state_t cur_dance(qk_tap_dance_state_t *state);

// Functions associated with individual tap dances
void ll_finished(qk_tap_dance_state_t *state, void *user_data);
void hl_finished(qk_tap_dance_state_t *state, void *user_data);
// void cs_finished(qk_tap_dance_state_t *state, void *user_data);
void ll_reset(qk_tap_dance_state_t *state, void *user_data);
void hl_reset(qk_tap_dance_state_t *state, void *user_data);
// void cs_reset(qk_tap_dance_state_t *state, void *user_data);

////////////////////////
/* Layers Definitions */
////////////////////////

enum layer_number {
    _DVORAK,
    _QWERTY,
    _PLOVER,
    _LOWER,
    _RAISE,
    _ADJUST,
};

enum base_layer_keycodes {
  DVORAK = SAFE_RANGE,
  QWERTY,
  PLOVER,
};

// lower/raise layer keycodes
#define LOWER TD(LOW_LAYR)
#define RAISE TD(HIGH_LAYR)

// @formatter:off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* DVORAK
   * ,-----------------------------------------.                    ,-----------------------------------------.
   * | ESC  | CAPS | HOME | PgDn | PgUp | End  |                    | Left | Down |  Up  |Right | INS  |  `   |
   * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
   * | Tab  |   '  |   ,  |   .  |   P  |   Y  |                    |   F  |   G  |   C  |   R  |   L  |  /   |
   * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
   * |LShift|   A  |   O  |   E  |   U  |   I  |-------.    ,-------|   D  |   H  |   T  |   N  |   S  |  _   |
   * |------+------+------+------+------+------|BackSP |    | DEL   |------+------+------+------+------+------|
   * |LALT  |   ;  |   Q  |   J  |   K  |   X  |-------|    |-------|   B  |   M  |   W  |   V  |   Z  |LShift|
   * `-----------------------------------------/       /     \      \-----------------------------------------'
   *                   |LGUI  |LCTRL |LOWER | /Space  /       \Enter \  |RAISE |LCTRL |LALT  |
   *                   |      |      |      |/       /         \      \ |      |      |      |
   *                   `----------------------------'           '------''--------------------'
   */
  [_DVORAK] = LAYOUT(
    KC_ESC,  KC_CAPS, KC_HOME, KC_PGDN, KC_PGUP, KC_END,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_INS,   KC_GRV,
    KC_TAB,  KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,                     KC_F,    KC_G,    KC_C,    KC_R,    KC_L,     KC_SLSH,
    KC_LSFT,  KC_A,   KC_O,    KC_E,    KC_U,    KC_I,                     KC_D,    KC_H,    KC_T,    KC_N,    KC_S,     KC_UNDS,
    KC_LALT, KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,  KC_BSPC,  KC_DEL,  KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,     KC_LSFT,
                             KC_LGUI, KC_LCTRL, LOWER,  KC_SPC,   KC_ENT,  RAISE, KC_LCTRL, KC_LALT
  ),
  /* QWERTY
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * | ESC  | CAPS | HOME | PgDn | PgUp | End  |                    | Left | Down |  Up  |Right | INS  |  `   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  _   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |LShift|   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
     * |------+------+------+------+------+------|BackSP |    | DEL   |------+------+------+------+------+------|
     * |LALT  |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |LShift|
     * `-----------------------------------------/       /     \      \-----------------------------------------'
     *                   |LGUI  |LCTRL |LOWER | /Space  /       \Enter \  |RAISE |LCTRL |LALT  |
     *                   |      |      |      |/       /         \      \ |      |      |      |
     *                   `----------------------------'           '------''--------------------'
   */
  [_QWERTY] = LAYOUT(
    KC_ESC,   KC_CAPS,KC_HOME, KC_PGDN, KC_PGUP, KC_END,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_INS,   KC_GRV,
    KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_UNDS,
    KC_LSFT,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT,
    KC_LALT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,  KC_BSPC,  KC_DEL,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_LSFT,
                             KC_LGUI, KC_LCTRL, LOWER,  KC_SPC,   KC_ENT,  RAISE, KC_LCTRL, KC_LALT
  ),
  /* PLOVER - Steno keyboard using Steno Protocol TX Bolt or GeminiPR (so that plover can use that and ignore key presses from other layers)
     * ,-----------------------------------------.                    ,-----------------------------------------.
     * | ESC  |  S   |  T   |  P   |  H   |  *   |                    |   *  |  F   |  P   |  L   |  T   |  D   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * | Tab  |  S   |  K   |  W   |  R   |  *   |                    |   *  |  R   |  B   |  G   |  S   |  Z   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |LShift| CAPS | HOME | PgDn | PgUp | End  |-------.    ,-------| Left | Down |  Up  |Right | INS  | Prnt |
     * |------+------+------+------+------+------|BackSP |    | DEL   |------+------+------+------+------+------|
     * |LALT  |GEMINI|TXBOLT|  #   |  A   |  O   |-------|    |-------|   E  |  U   |  #   |NK_TOGG|     |LShift|
     * `-----------------------------------------/       /     \      \-----------------------------------------'
     *                   |LGUI  |LCTRL |LOWER | /Space  /       \Enter \  |RAISE |LCTRL |LALT  |
     *                   |      |      |      |/       /         \      \ |      |      |      |
     *                   `----------------------------'           '------''--------------------'
   */
  [_PLOVER] = LAYOUT(
    KC_ESC,  STN_S1,          STN_TL,        STN_PL,  STN_HL,  STN_ST1,                  STN_ST3, STN_FR,  STN_PR, STN_LR,  STN_TR,  STN_DR,   
    KC_TAB,  STN_S2,          STN_KL,        STN_WL,  STN_RL,  STN_ST2,                  STN_ST4, STN_RR,  STN_BR, STN_GR,  STN_SR,  STN_ZR,
    KC_LSFT, KC_CAPS,         KC_HOME,       KC_PGDN, KC_PGUP, KC_END,                   KC_LEFT, KC_DOWN, KC_UP,  KC_RGHT, KC_INS,  KC_PSCR,
    KC_LALT, QK_STENO_GEMINI, QK_STENO_BOLT, STN_N1,  STN_A,   STN_O,   KC_BSPC, KC_DEL, STN_E,   STN_U,   STN_N2, NK_TOGG, XXXXXXX, KC_LSFT,
                                             KC_LGUI, KC_LCTRL, LOWER,  KC_SPC,  KC_ENT, RAISE, KC_LCTRL, KC_LALT
  ),
  /* LOWER - keys on lower and raise are divided into symmetrical halfs of same hand (hard shortcuts, hold infrequent or toggle) and other hand (easy shortcuts, hold)
   * ,-----------------------------------------.                    ,-----------------------------------------.
   * | ESC  | CAPS | HOME | PgDn | PgUp | End  |                    | Left | Down |  Up  |Right | INS  |  `   |
   * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
   * |   \  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |   ?  |
   * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
   * |LShift|   !  |   @  |   #  |   $  |   %  |-------.    ,-------|   ^  |   &  |   *  |   -  |   +  |   =  |
   * |------+------+------+------+------+------|BackSP |    | DEL   |------+------+------+------+------+------|
   * |LAlt  |  <   |   [  |   {  |   (  |  ~   |-------|    |-------|  |   |   )  |   }  |   ]  |   >  |LShift|
   * `-----------------------------------------/       /     \      \-----------------------------------------'
   *                   | LGUI |LCTRL |LOWER | /Space  /       \Enter \  |RAISE |LCTRL |LAlt  |
   *                   |      |      |      |/       /         \      \ |      |      |      |
   *                   `----------------------------'           '------''--------------------'
   *  OTHER HAND AND SO ASSIGN TO RAISE                                             OTHER HAND AND SO ASSIGN TO LOWER
   */
  [_LOWER] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                    _______, _______, _______, _______, _______, _______,
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                      KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_QUES,
    _______, KC_P1,   KC_P2,   KC_P3,   KC_P4,   KC_P5,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_MINS, KC_PLUS, KC_EQL,
    _______, KC_P6,   KC_P7,   KC_P8,   KC_P9,   KC_P0,   _______, _______,  KC_PIPE, KC_RPRN, KC_RCBR, KC_RBRC, KC_GT,   _______,
                               _______, _______, _______, _______, _______,  _______, _______, _______
  ),
  /* RAISE - keys on lower and raise are divided into symmetrical halfs of same hand (hard shortcuts, hold infrequent or toggle) and other hand (easy shortcuts, hold)
   * ,-----------------------------------------.                    ,-----------------------------------------.
   * | ESC  | CAPS | HOME | PgDn | PgUp | End  |                    | Left | Down |  Up  |Right | INS  | Prnt |
   * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
   * | F1   |  F2  |  F3  |  F4  |  F5  |  F6  |                    |  F7  |  F8  |  F9  | F10  | F11  | F12  |
   * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
   * |LShift| NUM1 | NUM2 | NUM3 | NUM4 | NUM5 |-------.    ,-------| NUM6 | NUM7 | NUM8 | NUM9 | NUM0 |RShift|
   * |------+------+------+------+------+------|BackSP |    | DEL   |------+------+------+------+------+------|
   * |LAlt  |MICMUT|PL/PAU| LAST | NEXT | NUMLK|-------|    |-------|SCRLCK|VOL_D |VOL_U | MUTE | RGUI | RAlt |
   * `-----------------------------------------/       /     \      \-----------------------------------------'
   *                   | LGUI |LCTRL |LOWER | /Space  /       \Enter \  |RAISE |RCTRL |LAlt  |
   *                   |      |      |      |/       /         \      \ |      |      |      |
   *                   `----------------------------'           '------''--------------------'
   *  SAME HAND AND SO ASSIGN TO LOWER                                             SAME HAND AND SO ASSIGN TO RAISE
   */
  [_RAISE] = LAYOUT(
    _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______,  KC_PSCR,
    KC_BSLS, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,
    _______, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,                      KC_NUM,  KC_VOLU, KC_MNXT, KC_MPLY, KC_P1,   KC_RSFT,
    _______, KC_LT,   KC_LBRC, KC_LCBR, KC_LPRN, KC_TILD, _______, _______, KC_SCRL, KC_VOLD, KC_MPRV, KC_MUTE, KC_RGUI,  KC_RALT,
                               _______, _______, _______, _______, _______, _______, _______, _______
  ),
  /* ADJUST - once memory permits will be used for multi key shortcuts
   * ,-----------------------------------------.                    ,-----------------------------------------.
   * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
   * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
   * |      |      |      |      |NK_TOGG|     |                    |      |QWERTY|      |      |      |      |
   * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
   * |      |      |      |      |PLOVER|      |-------.    ,-------|      |DVORAK|      |      |      |      |
   * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
   * |      |      |      |      |      |      |-------|    |-------|      |      |      |      |      |      |
   * `-----------------------------------------/       /     \      \-----------------------------------------'
   *                   | LGUI |LCTRL |LOWER | /Space  /       \Enter \  |RAISE |RCTRL | LAlt |
   *                   |      |      |      |/       /         \      \ |      |      |      |
   *                   `----------------------------'           '------''--------------------'
   */
  [_ADJUST] = LAYOUT(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, NK_TOGG, XXXXXXX,                   XXXXXXX, QWERTY,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, PLOVER,  XXXXXXX,                   XXXXXXX, DVORAK,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                               _______, _______, _______, _______, _______,  _______, _______, _______
  ),
};
// @formatter:on

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

///////////////////////////////
/* RGB Processing Functions */
///////////////////////////////

#ifdef RGBLIGHT_ENABLE
void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();                          // enables RGB, without saving settings
    rgblight_sethsv_noeeprom(HSV_RED);                   // sets the color to red without saving
    rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING + 3); // sets mode to Fast breathing without saving
}
#endif

//////////////////////////////////////////
/* Rotary Encoding Processing Functions */
//////////////////////////////////////////

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    // 0 is left-half encoder
    // 1 is right-half encoder
    if (index == 0) {
        tap_code(KC_0);
    } else if (index == 1) {
        tap_code(KC_1);
    }

    if (clockwise) {
        tap_code16(KC_PLUS);
    } else {
        tap_code(KC_MINUS);
    }

    return false;
}
#endif

///////////////////////////////
/* OLED Processing Functions */
///////////////////////////////

// SSD1306 OLED update loop, make sure to enable OLED_ENABLE=yes in rules.mk
//  A 128x32 OLED rotated 90 degrees is 5 characters wide and 16 characters tall
#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (!is_keyboard_master()) return OLED_ROTATION_180; // flips the display 180 degrees if offhand
    return OLED_ROTATION_270;
}

// When you add source files to SRC in rules.mk, you can use functions.
void set_label_dvorak(void);
void set_label_qwerty(void);
void set_label_plover(void);
const uint8_t render_lily_layer_state(uint8_t line);

void          add_to_keylog_queue(uint16_t keycode);
void          slow_clear_keylog_queue(void);
const uint8_t keylog_queue_empty(void);
const uint8_t render_keylog_queue(uint8_t line);

void          update_key_info(uint16_t keycode, keyrecord_t *record);
const uint8_t render_key_info(uint8_t line);

const uint8_t render_clap_logo(uint8_t line);
const uint8_t render_idle_logo(uint8_t line);

void set_keyboard_logo_to_dvorak(void);
void set_keyboard_logo_to_qwerty(void);
void set_keyboard_logo_to_plover(void);
const uint8_t render_keyboard_logo(uint8_t line);

bool oled_task_user(void) {
    // A 128x32 OLED rotated 90 degrees is 5 characters wide and 16 characters tall
    if (is_keyboard_master()) {
        uint8_t line = render_lily_layer_state(0);
        line = render_key_info(line);
        if (keylog_queue_empty()) {
            line = render_idle_logo(line);
        } else {
            line = render_clap_logo(line);
        }
        render_keylog_queue(line);
        slow_clear_keylog_queue();
    } else {
        render_keyboard_logo(0);
    }
    return false;
}
#endif // OLED_ENABLE

///////////////////////////////////
/* Keypress Processing Functions */
///////////////////////////////////

// Set-up defaults
void matrix_init_user(void) {
    steno_set_mode(STENO_MODE_GEMINI); // or STENO_MODE_BOLT
    set_label_dvorak();
    set_keyboard_logo_to_dvorak();
    set_single_persistent_default_layer(_DVORAK); // or _QWERTY or _PLOVER
}

// Intercept any key presses and post process them
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
#ifdef OLED_ENABLE
        update_key_info(keycode, record);
        add_to_keylog_queue(keycode);
#endif
        // set_timelog();
        // Allow escape to cancel tap dance layer toggles
        switch (keycode) {
            case DVORAK:
                if (record->event.pressed) {
                    set_label_dvorak();
                    set_keyboard_logo_to_dvorak();
                    set_single_persistent_default_layer(_DVORAK);
                    layer_off(_RAISE);
                    layer_off(_LOWER);
                    layer_off(_ADJUST);
                }
                return false;
                break;
            case QWERTY:
                if (record->event.pressed) {
                    set_label_qwerty();
                    set_keyboard_logo_to_qwerty();
                    set_single_persistent_default_layer(_QWERTY);
                    layer_off(_RAISE);
                    layer_off(_LOWER);
                    layer_off(_ADJUST);
                }
                return false;
                break;
             case PLOVER:
                if (record->event.pressed) {
                    set_label_plover();
                    set_keyboard_logo_to_plover();
                    set_single_persistent_default_layer(_PLOVER);
                    layer_off(_RAISE);
                    layer_off(_LOWER);
                    layer_off(_ADJUST);
                    if (!eeconfig_is_enabled()) {
                        eeconfig_init();
                    }
                    keymap_config.raw = eeconfig_read_keymap();
                    keymap_config.nkro = 1;
                    eeconfig_update_keymap(keymap_config.raw);
                }
                return false;
                break;
            case KC_ESC:
                layer_off(_RAISE);
                layer_off(_LOWER);
                break;
        }
    }

    return true;
}

/////////////////////////
/* Tap Dance Functions */
/////////////////////////

// Sets the delay that defines how long a tap is vs a hold
// per key
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD(LOW_LAYR):
            return 275;
        case TD(HIGH_LAYR):
            return 275;
        // case TD(CAPS_SHIFT):
        //   return 275;
        default:
            return TAPPING_TERM;
    }
}

// Determine the current tap dance state
td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count > 0) {
        if (!state->pressed)
            return TD_SINGLE_TAP;
        else
            return TD_SINGLE_HOLD;
    } // else if (state->count == 2) return TD_DOUBLE_TAP;
    else
        return TD_UNKNOWN;
}

// Initialize tap structure associated with lower layer
static td_tap_t ll_tap_state = {.is_press_action = true, .state = TD_NONE};

// Initialize tap structure associated with higher layer
static td_tap_t hl_tap_state = {.is_press_action = true, .state = TD_NONE};

// Initialize tap structure associated with caps tap/ shift hold
// static td_tap_t cs_tap_state = {
//    .is_press_action = true,
//    .state = TD_NONE
//};

// Function that controls lower layer key behaviour
void ll_finished(qk_tap_dance_state_t *state, void *user_data) {
    ll_tap_state.state = cur_dance(state);
    switch (ll_tap_state.state) {
        // case TD_DOUBLE_TAP:
        // case TD_SINGLE_TAP:
        //    tap_code(KC_QUOT);
        //    break;
        case TD_SINGLE_HOLD:
            layer_on(_LOWER);
            break;
        case TD_SINGLE_TAP:
            // Turn off other layer if on
            if (layer_state_is(_RAISE)) {
                layer_off(_RAISE);
                // Check to see if the layer is already set
            } else if (layer_state_is(_LOWER)) {
                // If already set, then switch it off
                layer_off(_LOWER);
            } else {
                // If not already set, then switch the layer on
                layer_on(_LOWER);
            }
            break;
        default:
            break;
    }
}

void ll_reset(qk_tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    if (ll_tap_state.state == TD_SINGLE_HOLD) {
        layer_off(_LOWER);
    }
    ll_tap_state.state = TD_NONE;
}

// Function that controls higher layer key behaviour
void hl_finished(qk_tap_dance_state_t *state, void *user_data) {
    hl_tap_state.state = cur_dance(state);
    switch (hl_tap_state.state) {
        // case TD_DOUBLE_TAP:
        // case TD_SINGLE_TAP:
        //    tap_code(KC_QUOT);
        //    break;
        case TD_SINGLE_HOLD:
            layer_on(_RAISE);
            break;
        case TD_SINGLE_TAP:
            // Turn off other layer if on
            if (layer_state_is(_LOWER)) {
                layer_off(_LOWER);
                // Check to see if the layer is already set
            } else if (layer_state_is(_RAISE)) {
                // If already set, then switch it off
                layer_off(_RAISE);
            } else {
                // If not already set, then switch the layer on
                layer_on(_RAISE);
            }
            break;
        default:
            break;
    }
}

void hl_reset(qk_tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    if (hl_tap_state.state == TD_SINGLE_HOLD) {
        layer_off(_RAISE);
    }
    hl_tap_state.state = TD_NONE;
}

// Function that controls caps/shift key behaviour
// void cs_finished(qk_tap_dance_state_t *state, void *user_data) {
//    cs_tap_state.state = cur_dance(state);
//    switch (cs_tap_state.state) {
//        // case TD_DOUBLE_TAP:
//        // case TD_SINGLE_TAP:
//        //    tap_code(KC_QUOT);
//        //    break;
//        case TD_SINGLE_HOLD:
//	    register_code(KC_LSFT);
//            break;
//        case TD_SINGLE_TAP:
//	    tap_code(KC_CAPS);
//            break;
//        default:
//            break;
//    }
//}

// void cs_reset(qk_tap_dance_state_t *state, void *user_data) {
//     // If the key was held down and now is released then switch off the layer
//     if (cs_tap_state.state == TD_SINGLE_HOLD) {
//	unregister_code(KC_LSFT);
//     }
//     cs_tap_state.state = TD_NONE;
// }

// Associate our tap dance key with its functionality
qk_tap_dance_action_t tap_dance_actions[] = {
    [LOW_LAYR]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ll_finished, ll_reset),
    [HIGH_LAYR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, hl_finished, hl_reset),
    // [KC_LSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, cs_finished, cs_reset),
};
