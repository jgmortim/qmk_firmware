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
#include "config.h"

extern rgb_config_t rgb_matrix_config;

bool rgb_enabled_flag;                  // Current LED state flag. If false then LED is off.
bool rgb_time_out_enable;               // Idle LED toggle enable. If false then LED will not turn off after idle timeout.
uint16_t rgb_time_out_seconds;          // Idle LED timeout value, in seconds not milliseconds
led_flags_t rgb_time_out_saved_flag;    // Store LED flag before timeout so it can be restored when LED is turned on again.

uint8_t os_mode;                        // Stores the current OS mode.
bool os_mode_led_flag;                  // Current OS indicator LED state flag. If false, then the LED is off.

uint8_t audio_output;                   // Stores the current audio output device. Initialized to the first device.

enum layout_names {
    _KL=0,       // Keys Layout: The main keyboard layout that has all the characters
    _FL,         // Function Layout: The function key activated layout with default functions
    _NL,         // Numpad Layout: Adds a numpad
    _SL          // Spanish Layout: Spanish accented letter macros
};

enum ctrl_keycodes {
    U_T_AUTO = SAFE_RANGE, // USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,              // USB Toggle Automatic GCR control
    DBG_TOG,               // DEBUG Toggle On / Off
    DBG_MTRX,              // DEBUG Toggle Matrix Prints
    DBG_KBD,               // DEBUG Toggle Keyboard Prints
    DBG_MOU,               // DEBUG Toggle Mouse Prints
    MD_BOOT,               // Restart into bootloader after hold timeout
};

enum custom_keycodes {
    // The start of this enum should always be equal to end of ctrl_keycodes + 1
    // Accented letters must all be grouped together and be implemented in accent_table.
    A_ACUTE = MD_BOOT + 1, // A with acute accent
    E_ACUTE,               // E with acute accent
    I_ACUTE,               // I with acute accent
    O_ACUTE,               // O with acute accent
    U_ACUTE,               // U with acute accent
    ENE,                   // N with tilde accent
    INV_EXC,               // Inverted exclamation point
    INV_QUS,               // Inverted question mark
    OS_TOG                 // Toggle OS mode between Windows and Linux
};

enum tapdance_keycodes {
    TD_ALT_SL = 0, // Tap dance key to switch to Spanish layer
    TD_GRV_NL      // Tap dance key to switch to Numpad layer
};

enum os {
    WINDOWS = 0,
    LINUX
};

typedef struct {
    const char *win_norm;    // Windows alt code for lowercase
    const char *win_shift;   // Windows alt code for uppercase
    const char *linux_norm;  // Linux unicode for lowercase
    const char *linux_shift; // Linux unicode for uppercase
} accent_entry_t;

static const accent_entry_t accent_table[] = {
    // Must be in the same order as in custom_keycodes.
    {"0225", "0193", "00E1", "00C1" }, // A_ACUTE
    {"0233", "0201", "00E9", "00C9" }, // E_ACUTE
    {"0237", "0205", "00ED", "00CD" }, // I_ACUTE
    {"0243", "0211", "00F3", "00D3" }, // O_ACUTE
    {"0250", "0218", "00FA", "00DA" }, // U_ACUTE
    {"0241", "0209", "00F1", "00D1" }, // ENE
};

#define ACCENT_OFFSET A_ACUTE

static uint16_t idle_timer;             // Idle LED timeout timer
static uint16_t idle_second_counter;    // Idle LED seconds counter, counts seconds not milliseconds
static uint8_t key_event_counter;       // This counter is used to check if any keys are being held

static uint16_t os_ind_led_timer;          // OS toggle LED timeout timer
static uint16_t os_ind_led_second_counter; // OS toggle LED seconds counter, counts seconds not milliseconds

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
        _______, OS_TOG,   _______,                   _______,                            _______, _______, _______, _______,          _______, _______, _______
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
        ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,         ______, ______, ______,
        ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______, ______, ______, ______, ______,
        ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______, ______, ______, ______, ______,
        ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,
        ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,   ______,                           ______,
        ______,   ______,   ______,                       ______,                                 ______,   ______,   ______,   ______,         ______, ______, ______,
        /* Boarder; starts bottom right and moves clockwise */
        EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED,
        EDGE_LED, EDGE_LED, EDGE_LED,
        EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED, EDGE_LED,
        EDGE_LED, EDGE_LED, EDGE_LED
    },
    [_FL] = {
        ______, ______, ______, ______, ______, ______,  ______,  ______,  ______,  ______, ______, ______, ______,         ______, ______, ______,
        ______, ______, ______, ______, ______, ______,  ______,  ______,  ______,  ______, ______, ______, ______, ______, ______, ______, ______,
        ______, ______, ______, ______, ______, ______,  ______,  KEY_LED, KEY_LED, ______, ______, ______, ______, ______, ______, ______, ______,
        ______, ______, ______, ______, ______, ______,  ______,  ______,  ______,  ______, ______, ______, ______,
        ______, ______, ______, ______, ______, KEY_LED, KEY_LED, ______,  ______,  ______, ______, ______,                         ______,
        ______, ______, ______,                 ______,                             ______, ______, ______, ______,         ______, ______, ______
    },
    [_NL] = {
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,           KEY_LED, KEY_LED, KEY_LED,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,  ______,  KEY_LED, KEY_LED, KEY_LED,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,  KEY_LED, KEY_LED, KEY_LED, KEY_LED,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, KEY_LED,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,                            ______,
        ______, ______, ______,                 ______,                         ______, ______, ______, ______,           ______,  ______,  ______
    },
    [_SL] = {
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,         ______, ______, ______,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,
        ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______, ______,                         ______,
        ______, ______, SP_IND,                 ______,                         ______, ______, ______, ______,         ______, ______, ______
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
    os_mode = WINDOWS;                                  // Default to Windows mode.
    os_mode_led_flag = false;                           // Default OS indicator LED to off.
    audio_output = 0;                                   // Assume the first device is selected.
};

/* Runs just one time after everything else has initialized. */
void keyboard_post_init_user(void) {
    rgb_matrix_enable();
}

void handle_rgb_timeout(void) {
    if (rgb_time_out_enable && rgb_enabled_flag) {
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
}

void handle_os_mode_led_timeout(void) {
    if(os_mode_led_flag) {
        // Update the second counter after each second.
        if (timer_elapsed(os_ind_led_timer) > MILLISECONDS_IN_SECOND) {
            os_ind_led_second_counter++;
            os_ind_led_timer = timer_read();
        }

        // Turn off OS indicator LED after timeout reached.
        if (os_ind_led_second_counter >= OS_MODE_IND_TIME_OUT) {
            os_mode_led_flag = false;
            os_ind_led_second_counter = 0;
        }
    }
}

/* Runs constantly in the background, in a loop. */
void matrix_scan_user(void) {
    handle_rgb_timeout();
    handle_os_mode_led_timeout();
};

/* Sends the given accented letter. */
void send_accent(uint16_t keycode) {
    uint16_t index = keycode - ACCENT_OFFSET;
    if (index >= ARRAY_SIZE(accent_table)) return; // out of range

    const accent_entry_t *entry = &accent_table[index];
    const bool uppercase = MODS_SHIFT || host_keyboard_led_state().caps_lock;

    if (os_mode == WINDOWS) {
        send_alt_code(uppercase ? entry->win_shift : entry->win_norm);
    } else {
        send_unicode(uppercase ? entry->linux_shift : entry->linux_norm);
    }
}

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
        /* Toggle OS mode between Windows and Linux */
        case OS_TOG:
            if (record->event.pressed) {
                os_mode ^= (WINDOWS | LINUX);
                os_mode_led_flag = true;
                os_ind_led_second_counter = 0;
                return false;
            }
            return true;
        /* Change Win + c to be Calculator instead of Cortana */
        case KC_C:
            if (record->event.pressed && get_mods() == MOD_BIT(KC_LGUI)) {
                os_mode == WINDOWS
                    ? windows_run("CALC")
                    : linux_run("gnome-calculator");
                return false;
            }
            return true;
        /* Change Win + s to be Snipping Tool instead of search */
        case KC_S:
            if (record->event.pressed && get_mods() == MOD_BIT(KC_LGUI) && os_mode == WINDOWS) {
                windows_run("SnippingTool");
                return false;
            }
            return true;
        /* Change Win + a to toggle audio output devices instead of Action Center */
        case KC_A:
            if (record->event.pressed && get_mods() == MOD_BIT(KC_LGUI) && os_mode == WINDOWS) {
                audio_output = (audio_output + 1) % NUMBER_OF_AUDIO_OUTPUT_DEVICES; // Increment to the next device.
                SEND_STRING(SS_LGUI(SS_LCTL("v")) SS_DELAY(200)); // Open the sound output page of quick settings.
                for (int i = 0; i < audio_output; i++) {
                    SEND_STRING(SS_TAP(X_DOWN)); // Press the down arrow until the appropriate device is highlighted.
                }
                SEND_STRING(SS_TAP(X_ENT) SS_TAP(X_ESC)); // Make the selection and close the menu.
                return false;
            }
            return true;
        /* Spanish Letters and Punctuation */
        case A_ACUTE ... ENE: // Any of the Spanish accented letters
            if (record->event.pressed) {
                send_accent(keycode);
                layer_off(_SL);
            }
            return false;
        case INV_EXC:
            if (record->event.pressed) {
                if (MODS_SHIFT) { // Inverted exclamation point
                    if (os_mode == WINDOWS) {
                        send_alt_code("0161");
                    } else {
                        send_unicode("00A1");
                    }
                    layer_off(_SL);
                } else {
                    SEND_STRING("1");
                }
            }
            return false;
        case INV_QUS:
            if (record->event.pressed) {
                if (MODS_SHIFT) { // Inverted question mark
                    if (os_mode == WINDOWS) {
                        send_alt_code("0191");
                    } else {
                        send_unicode("00BF");
                    }
                    layer_off(_SL);
                } else {
                    SEND_STRING("/");
                }
            }
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

/* Runs everytime a layer change happens */
layer_state_t layer_state_set_user(layer_state_t state) {
    /* When turning on the numpad, make sure NUM LOCK is on */
    if (get_highest_layer(state) == _NL && !host_keyboard_led_state().num_lock) {
        tap_code(KC_NUM);
    }
    /* No special logic for the other layers */
    return state;
}

/* Helper: set an LED from HSV with brightness scaling */
static void set_led_hsv(uint8_t index, HSV hsv) {
    RGB rgb = hsv_to_rgb(hsv);
    float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
    rgb_matrix_set_color(index, f * rgb.r, f * rgb.g, f * rgb.b);
}

void set_layer_color(int layer) {
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        /* Special Indicator Logic */
        if (i == OS_MODE_IND_LED && (os_mode_led_flag || IS_LAYER_ON(_FL))) { // OS indicator LED
            set_led_hsv(i, os_mode == WINDOWS ? (HSV)WIN_IND : (HSV)LNX_IND);
            continue;
        }
        if (i == CAPS_LOCK_IND_LED && host_keyboard_led_state().caps_lock) { // Caps Lock indicator LED
            set_led_hsv(i, (HSV)CAPS_IND);
            continue;
        }
        if (i == SCRL_LOCK_IND_LED && host_keyboard_led_state().scroll_lock) { // Scroll Lock indicator LED
            set_led_hsv(i, (HSV)SCRL_IND);
            continue;
        }
        /* Normal Logic */
        HSV hsv = {
            .h = pgm_read_byte(&ledmap[layer][i][0]),
            .s = pgm_read_byte(&ledmap[layer][i][1]),
            .v = pgm_read_byte(&ledmap[layer][i][2]),
        };
        if (hsv.h || hsv.s || hsv.v) {
            set_led_hsv(i, hsv);
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
                        set_led_hsv(i, lower_hsv);
                        is_undefined = false;
                        break;
                    }
                }
            }

            if (is_undefined) {
                // If the values are all false then it's a transparent key and the LED should be deactivated.
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
