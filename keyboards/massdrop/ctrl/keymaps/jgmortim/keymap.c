/*
QMK Firmware Massdrop CTRL jgmortim Keymap
Copyright (C) 2025 jgmortim

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#define MILLISECONDS_IN_SECOND 1000

#define COPPER {12, 255, 100}
#define CPPR_BRT {12, 255, 225}
#define CYAN {HSV_CYAN}
#define ______ {HSV_BLACK} // 5 underscores instead of the 6 used by the KC_TRNS alias.

#define RGB_TIME_OUT 300  // 300 seconds (5 minutes).

extern rgb_config_t rgb_matrix_config;

bool rgb_enabled_flag;                  // Current LED state flag. If false then LED is off.
bool rgb_time_out_enable;               // Idle LED toggle enable. If false then LED will not turn off after idle timeout.
bool rgb_time_out_user_value;           // This holds the toggle value set by user with ROUT_TG. It's necessary as RGB_TOG changes timeout enable.
uint16_t rgb_time_out_seconds;          // Idle LED timeout value, in seconds not milliseconds
led_flags_t rgb_time_out_saved_flag;    // Store LED flag before timeout so it can be restored when LED is turned on again.

enum tapdance_keycodes {
    TD_ALT_SL = 0, // Tap dance key to switch to Spanish layer
    TD_GRV_NL      // Tap dance key to switch to Numpad layer
};

enum layout_names {
    _KL=0,       // Keys Layout: The main keyboard layout that has all the characters
    _FL,         // Function Layout: The function key activated layout with default functions
    _NL,         // Numpad Layout: Adds a numpad
    _SL          // Spanish Layout: Spanish accented letter macros
};

enum ctrl_keycodes {
    U_T_AUTO = SAFE_RANGE, //USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,              //USB Toggle Automatic GCR control
    DBG_TOG,               //DEBUG Toggle On / Off
    DBG_MTRX,              //DEBUG Toggle Matrix Prints
    DBG_KBD,               //DEBUG Toggle Keyboard Prints
    DBG_MOU,               //DEBUG Toggle Mouse Prints
    MD_BOOT,               //Restart into bootloader after hold timeout
};

enum custom_keycodes {
    // The start of this enum should always be equal to end of ctrl_keycodes + 1
    A_ACUTE = MD_BOOT + 1, // A with acute accent
    E_ACUTE,               // E with acute accent
    I_ACUTE,               // I with acute accent
    O_ACUTE,               // O with acute accent
    U_ACUTE,               // U with acute accent
    ENE,                   // N with tilde accent
    INV_EXC,               // Inverted exclamation point
    INV_QUS                // Inverted question mark
};

static uint16_t idle_timer;             // Idle LED timeout timer
static uint16_t idle_second_counter;    // Idle LED seconds counter, counts seconds not milliseconds
static uint8_t key_event_counter;       // This counter is used to check if any keys are being held

/* Associate tap dance keys with their functionality */
tap_dance_action_t tap_dance_actions[] = {
    [TD_ALT_SL] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_LALT, _SL),
    [TD_GRV_NL] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_GRV, _NL)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_KL] = LAYOUT(
        KC_ESC,       KC_F1,   KC_F2,         KC_F3, KC_F4, KC_F5,  KC_F6, KC_F7, KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,           KC_PSCR, KC_SCRL, KC_PAUS,
        TD(TD_GRV_NL), KC_1,   KC_2,          KC_3,  KC_4,  KC_5,   KC_6,  KC_7,  KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_INS,  KC_HOME, KC_PGUP,
        KC_TAB,       KC_Q,    KC_W,          KC_E,  KC_R,  KC_T,   KC_Y,  KC_U,  KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL,  KC_END,  KC_PGDN,
        KC_CAPS,      KC_A,    KC_S,          KC_D,  KC_F,  KC_G,   KC_H,  KC_J,  KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT,      KC_Z,    KC_X,          KC_C,  KC_V,  KC_B,   KC_N,  KC_M,  KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,                            KC_UP,
        KC_LCTL,      KC_LGUI, TD(TD_ALT_SL),               KC_SPC,                        KC_RALT, MO(1),   KC_APP,  KC_RCTL,          KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [_FL] = LAYOUT(
        _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_MUTE, _______, _______,
        _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MPLY, KC_MSTP, KC_VOLU,
        _______, _______,  _______, _______, _______, _______, _______, U_T_AUTO,U_T_AGCR,_______, _______, _______, _______, _______, KC_MPRV, KC_MNXT, KC_VOLD,
        _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______,  _______, _______, _______, MD_BOOT, NK_TOGG, _______, _______, _______, _______, _______,                            _______,
        _______, _______,  _______,                   _______,                            _______, _______, _______, _______,          _______, _______, _______
    ),
    [_NL] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_KP_7, KC_KP_8, KC_KP_9,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_KP_4, KC_KP_5, KC_KP_6,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_KP_0, KC_KP_1, KC_KP_2, KC_KP_3,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                            _______,
        _______, _______, _______,                   _______,                            _______, _______, _______, _______,          _______, _______, _______
    ),
    [_SL] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, INV_EXC, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, E_ACUTE, _______, _______, _______, U_ACUTE, I_ACUTE, O_ACUTE, _______, _______, _______, _______, _______, _______, _______,
        _______, A_ACUTE, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, ENE,     _______, _______, _______, INV_QUS, _______,                            _______,
        _______, _______, _______,                   _______,                            _______, _______, _______, _______,          _______, _______, _______
    )
};

const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
    [_KL] = {
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,         ______, ______, ______,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,                         ______,
        ______, ______, ______,                 ______,                         ______, ______, ______, ______,         ______, ______, ______,
        /* Boarder; starts bottom right and moves clockwise */
        COPPER, COPPER, COPPER, COPPER, COPPER, COPPER, COPPER, COPPER, COPPER, COPPER, COPPER, COPPER, COPPER,
        COPPER, COPPER, COPPER,
        COPPER, COPPER, COPPER, COPPER, COPPER, COPPER, COPPER, COPPER, COPPER, COPPER, COPPER, COPPER, COPPER,
        COPPER, COPPER, COPPER
    },
    [_FL] = {
        ______, ______, ______, ______, ______, ______,   ______,   ______,   ______,   ______, ______, ______, ______,         ______, ______, ______,
        ______, ______, ______, ______, ______, ______,   ______,   ______,   ______,   ______, ______, ______, ______, ______, ______, ______, ______,
        ______, ______, ______, ______, ______, ______,   ______,   CPPR_BRT, CPPR_BRT, ______, ______, ______, ______, ______, ______, ______, ______,
        ______, ______, ______, ______, ______, ______,   ______,   ______,   ______,   ______, ______, ______, ______,
        ______, ______, ______, ______, ______, CPPR_BRT, CPPR_BRT, ______,   ______,   ______, ______, ______,                         ______,
        ______, ______, ______,                 ______,                                 ______, ______, ______, ______,         ______, ______, ______
    },
    [_NL] = {
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,             CPPR_BRT, CPPR_BRT, CPPR_BRT,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,   ______,   CPPR_BRT, CPPR_BRT, CPPR_BRT,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,   CPPR_BRT, CPPR_BRT, CPPR_BRT, CPPR_BRT,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, CPPR_BRT,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,                               ______,
        ______, ______, ______,                 ______,                         ______, ______, ______, ______,             ______,   ______,   ______
    },
    [_SL] = {
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,         ______, ______, ______,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,                         ______,
        ______, ______, CYAN,                   ______,                         ______, ______, ______, ______,         ______, ______, ______
    }
};

/* Runs just one time when the keyboard initializes. */
void matrix_init_user(void) {
    idle_second_counter = 0;                            // Counter for number of seconds keyboard has been idle.
    key_event_counter = 0;                              // Counter to determine if keys are being held, neutral at 0.
    rgb_time_out_seconds = RGB_TIME_OUT;                // RGB timeout initialized to its default.
    rgb_time_out_enable = true;                         // Enable RGB timeout by default.
    rgb_enabled_flag = true;                            // Initially, keyboard RGB is enabled.
    rgb_time_out_saved_flag = rgb_matrix_get_flags();   // Save RGB matrix state for when keyboard comes back from ide.
};

/* Runs just one time after everything else has initialized. */
void keyboard_post_init_user(void) {
    rgb_matrix_enable();
}

/* Runs constantly in the background, in a loop. */
void matrix_scan_user(void) {
    if(rgb_time_out_enable && rgb_enabled_flag) {
        // If the key event counter is not zero then some key was pressed down but not released, thus reset the timeout counter.
        if (key_event_counter) {
            idle_second_counter = 0;
        } else if (timer_elapsed(idle_timer) > MILLISECONDS_IN_SECOND) {
            idle_second_counter++;
            idle_timer = timer_read();
        }

        if (idle_second_counter >= rgb_time_out_seconds) {
            rgb_time_out_saved_flag = rgb_matrix_get_flags();
            rgb_matrix_set_flags(LED_FLAG_NONE);
            rgb_matrix_disable_noeeprom();
            rgb_enabled_flag = false;
            idle_second_counter = 0;
        }
    }
};

/* Executes the given alt code. */
void send_alt_code(char code[]) {
    uint8_t mod_state = get_mods(); // Gets the current mods
    unregister_mods(mod_state);     // Turns all active mods off

    SEND_STRING(SS_DOWN(X_LALT));
    for(int i = 0; code[i] != '\0';  i++) {
        switch(code[i]) {
            case '1': SEND_STRING(SS_TAP(X_KP_1)); break;
            case '2': SEND_STRING(SS_TAP(X_KP_2)); break;
            case '3': SEND_STRING(SS_TAP(X_KP_3)); break;
            case '4': SEND_STRING(SS_TAP(X_KP_4)); break;
            case '5': SEND_STRING(SS_TAP(X_KP_5)); break;
            case '6': SEND_STRING(SS_TAP(X_KP_6)); break;
            case '7': SEND_STRING(SS_TAP(X_KP_7)); break;
            case '8': SEND_STRING(SS_TAP(X_KP_8)); break;
            case '9': SEND_STRING(SS_TAP(X_KP_9)); break;
            default:  SEND_STRING(SS_TAP(X_KP_0)); break;
        }
    }
    SEND_STRING(SS_UP(X_LALT));

    register_mods(mod_state); // Restores the mods to their original state
}

/* Enters the given command into the Windows Run dialog. */
void windows_run(const char *command) {
    SEND_STRING(SS_DOWN(X_LGUI) SS_TAP(X_R) SS_UP(X_LGUI) SS_DELAY(50)); // Open the run dialog,
    SEND_STRING(command);                                                // type in the command,
    SEND_STRING(SS_TAP(X_ENT));                                          // and press enter.
}

#define MODS_SHIFT  (get_mods() & MOD_MASK_SHIFT)
#define MODS_CTRL   (get_mods() & MOD_MASK_CTRL)
#define MODS_ALT    (get_mods() & MOD_MASK_ALT)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;

    // Increment key event counter for every press and decrement for every release.
    if (record->event.pressed) {
        key_event_counter++;
    } else {
        key_event_counter--;
    }

    if (rgb_time_out_enable) {
        idle_timer = timer_read();
        // Reset the seconds counter. Without this, something like press> leave x seconds> press, would be x seconds on the effective counter not 0 as it should.
        idle_second_counter = 0;
        if (!rgb_enabled_flag) {
            rgb_matrix_enable_noeeprom();
            rgb_matrix_set_flags(rgb_time_out_saved_flag);
            rgb_enabled_flag = true;
        }
    }

    switch (keycode) {
        /* Change Win + c to be Calculator instead of Cortana */
        case KC_C:
            if (record->event.pressed && get_mods() == MOD_BIT(KC_LGUI)) {
                windows_run("CALC");
                return false;
            }
            return true;
        /* Change Win + s to be Snipping Tool instead of search */
        case KC_S:
            if (record->event.pressed && get_mods() == MOD_BIT(KC_LGUI)) {
                windows_run("SnippingTool");
                return false;
            }
            return true;
        /* Spanish Letters and Punctuation */
        case A_ACUTE:
            if (record->event.pressed) {
                MODS_SHIFT
                    ? send_alt_code("0193")  // Uppercase A with acute accent: ALT + 0193
                    : send_alt_code("0225"); // Lowercase a with acute accent: ALT + 0225
            }
            layer_off(3);
            return false;
        case E_ACUTE:
            if (record->event.pressed) {
                MODS_SHIFT
                    ? send_alt_code("0201")  // Uppercase E with acute accent: ALT + 0201
                    : send_alt_code("0233"); // Lowercase e with acute accent: ALT + 0233
            }
            layer_off(3);
            return false;
        case I_ACUTE:
            if (record->event.pressed) {
                MODS_SHIFT
                    ? send_alt_code("0205")  // Uppercase I with acute accent: ALT + 0205
                    : send_alt_code("0237"); // Lowercase i with acute accent: ALT + 0237
            }
            layer_off(3);
            return false;
        case O_ACUTE:
            if (record->event.pressed) {
                MODS_SHIFT
                    ? send_alt_code("0211")  // Uppercase O with acute accent: ALT + 0211
                    : send_alt_code("0243"); // Lowercase o with acute accent: ALT + 0243
            }
            layer_off(3);
            return false;
        case U_ACUTE:
            if (record->event.pressed) {
                MODS_SHIFT
                    ? send_alt_code("0218")  // Uppercase U with acute accent: ALT + 0218
                    : send_alt_code("0250"); // Lowercase u with acute accent: ALT + 0250
            }
            layer_off(3);
            return false;
        case ENE:
            if (record->event.pressed) {
                MODS_SHIFT
                    ? send_alt_code("0209")  // Uppercase N with tilde accent: ALT + 0209
                    : send_alt_code("0241"); // Lowercase n with tilde accent: ALT + 0241
            }
            layer_off(3);
            return false;
        case INV_EXC:
            if (record->event.pressed) {
                MODS_SHIFT
                    ? send_alt_code("0161")  // Inverted exclamation point: ALT + 0161
                    : SEND_STRING("1");
            }
            layer_off(3);
            return false;
        case INV_QUS:
            if (record->event.pressed) {
                MODS_SHIFT
                    ? send_alt_code("0191")  // Inverted question mark: ALT + 0191
                    : SEND_STRING("/");
            }
            layer_off(3);
            return false;
        /* Defaults */
        case U_T_AUTO:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_extra_manual, "USB extra port manual mode");
            }
            return false;
        case U_T_AGCR:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_gcr_auto, "USB GCR auto mode");
            }
            return false;
        case DBG_TOG:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_enable, "Debug mode");
            }
            return false;
        case DBG_MTRX:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_matrix, "Debug matrix");
            }
            return false;
        case DBG_KBD:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_keyboard, "Debug keyboard");
            }
            return false;
        case DBG_MOU:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_mouse, "Debug mouse");
            }
            return false;
        case MD_BOOT:
            if (record->event.pressed) {
                key_timer = timer_read32();
            } else {
                if (timer_elapsed32(key_timer) >= 500) {
                    reset_keyboard();
                }
            }
            return false;
        default:
            return true; //Process all other keycodes normally
    }
}

void set_layer_color(int layer) {
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        HSV hsv = {
            .h = pgm_read_byte(&ledmap[layer][i][0]),
            .s = pgm_read_byte(&ledmap[layer][i][1]),
            .v = pgm_read_byte(&ledmap[layer][i][2]),
        };
        if (hsv.h || hsv.s || hsv.v) {
            RGB rgb = hsv_to_rgb(hsv);
            float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
            rgb_matrix_set_color(i, f * rgb.r, f * rgb.g, f * rgb.b);
        } else {
            bool is_undefined = true;

            // Attempt to get HSV values from a lower active layer, working down from the current layer.
            for (int lower_layer_j = layer - 1; lower_layer_j >= 0; lower_layer_j--) {
                // IS_LAYER_ON(_KL) returns false for some reason, might have something to do with it being the default
                // layer. For my purposes, _KL is always active so I added "|| lower_layer_j == _KL".
                if (IS_LAYER_ON(lower_layer_j) || lower_layer_j == _KL) {
                    HSV lower_hsv = {
                        .h = pgm_read_byte(&ledmap[lower_layer_j][i][0]),
                        .s = pgm_read_byte(&ledmap[lower_layer_j][i][1]),
                        .v = pgm_read_byte(&ledmap[lower_layer_j][i][2]),
                    };
                    if (lower_hsv.h || lower_hsv.s || lower_hsv.v) {
                        RGB rgb = hsv_to_rgb(lower_hsv);
                        float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
                        rgb_matrix_set_color(i, f * rgb.r, f * rgb.g, f * rgb.b);
                        is_undefined = false;
                        break;
                    }
                }
            }

            if (is_undefined) {
                // If the values are all false then it's a transparent key and deactivate LED at this layer
                rgb_matrix_set_color(i, 0, 0, 0);
            }
        }
    }
}

bool rgb_matrix_indicators_user(void) {
    if (rgb_matrix_get_flags() == LED_FLAG_NONE ||
        rgb_matrix_get_flags() == LED_FLAG_UNDERGLOW) {
            return true;
        }
    set_layer_color(get_highest_layer(layer_state));
    return false;
}
