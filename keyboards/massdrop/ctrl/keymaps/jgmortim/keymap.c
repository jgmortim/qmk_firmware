#include QMK_KEYBOARD_H

#define BLACK {HSV_BLACK}
#define CYAN {HSV_CYAN}
#define DIM_RED {0, 255, 100}
#define DIM_WHITE {0, 0, 100}

extern rgb_config_t rgb_matrix_config;
bool rgb_enabled_flag;                  // Current LED state flag. If false then LED is off.

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
    A_AC_LOW = MD_BOOT + 1, // Lowercase a with acute accent
    A_AC_CAP,               // Uppercase A with acute accent
    E_AC_LOW,               // Lowercase e with acute accent
    E_AC_CAP,               // Uppercase E with acute accent
    I_AC_LOW,               // Lowercase i with acute accent
    I_AC_CAP,               // Uppercase I with acute accent
    O_AC_LOW,               // Lowercase o with acute accent
    O_AC_CAP,               // Uppercase O with acute accent
    U_AC_LOW,               // Lowercase u with acute accent
    U_AC_CAP,               // Uppercase U with acute accent
    ENE_LOW,                // Lowercase n with tilde accent
    ENE_CAP,                // Uppercase N with tilde accent
    INV_EXC,                // Inverted exclamation point
    INV_QUS                 // Inverted question mark
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC,        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,             KC_PSCR, KC_SCRL, KC_PAUS,
        LT(2,KC_GRV),  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,   KC_INS,  KC_HOME, KC_PGUP,
        KC_TAB,        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,   KC_DEL,  KC_END,  KC_PGDN,
        LT(3,KC_CAPS), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT,       KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,                              KC_UP,
        KC_LCTL,       KC_LGUI, KC_LALT,                   KC_SPC,                             KC_RALT, MO(1),   KC_APP,  KC_RCTL,            KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [1] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,            KC_MUTE, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   KC_MPLY, KC_MSTP, KC_VOLU,
        _______, RGB_SPD, RGB_VAI, RGB_SPI, RGB_HUI, RGB_SAI, _______, U_T_AUTO,U_T_AGCR,_______, _______, _______, _______, _______,   KC_MPRV, KC_MNXT, KC_VOLD,
        _______, RGB_RMOD,RGB_VAD, RGB_MOD, RGB_HUD, RGB_SAD, _______, _______, _______, _______, _______, _______, _______,
        _______, RGB_TOG, _______, _______, _______, MD_BOOT, NK_TOGG, _______, _______, _______, _______, _______,                              _______,
        _______, _______, _______,                   _______,                            _______, _______, _______, _______,            _______, _______, _______
    ),
    [2] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,            KC_KP_7, KC_KP_8, KC_KP_9,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   KC_KP_4, KC_KP_5, KC_KP_6,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_KP_0,   KC_KP_1, KC_KP_2, KC_KP_3,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                              _______,
        _______, _______, _______,                   _______,                            _______, _______, _______, _______,            _______, _______, _______
    ),
    [3] = LAYOUT(
        _______,       _______,  _______, _______,  _______, _______, _______, _______,  _______,  _______,  _______, _______, _______,            _______, _______, _______,
        _______,       _______,  _______, _______,  _______, _______, _______, _______,  _______,  _______,  _______, _______, _______, _______,   _______, _______, _______,
        _______,       _______,  _______, E_AC_LOW, _______, _______, _______, U_AC_LOW, I_AC_LOW, O_AC_LOW, _______, _______, _______, _______,   _______, _______, _______,
        _______,       A_AC_LOW, _______, _______,  _______, _______, _______, _______,  _______,  _______,  _______, _______, _______,
        LT(4,KC_TRNS), _______,  _______, _______,  _______, _______, ENE_LOW, _______,  _______,  _______,  _______, _______,                              _______,
        _______,       _______,  _______,                    _______,                              _______,  _______, _______, _______,            _______, _______, _______
    ),
    [4] = LAYOUT(
        _______,       _______,  _______, _______,  _______, _______, _______, _______,  _______,  _______,  _______, _______, _______,            _______, _______, _______,
        _______,       INV_EXC,  _______, _______,  _______, _______, _______, _______,  _______,  _______,  _______, _______, _______, _______,   _______, _______, _______,
        _______,       _______,  _______, E_AC_CAP, _______, _______, _______, U_AC_CAP, I_AC_CAP, O_AC_CAP, _______, _______, _______, _______,   _______, _______, _______,
        _______,       A_AC_CAP, _______, _______,  _______, _______, _______, _______,  _______,  _______,  _______, _______, _______,
        _______,       _______,  _______, _______,  _______, _______, ENE_CAP, _______,  _______,  _______,  INV_QUS, _______,                              _______,
        _______,       _______,  _______,                    _______,                              _______,  _______, _______, _______,            _______, _______, _______
    )
};

const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
    [0] = {
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,        BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,                      BLACK,
        BLACK, BLACK, BLACK,               BLACK,                      BLACK, BLACK, BLACK, BLACK,        BLACK, BLACK, BLACK,
        // Boarder; starts bottom right and moves clockwise
        DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED,
        DIM_RED, DIM_WHITE, DIM_RED,
        DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED,
        DIM_RED, DIM_WHITE, DIM_RED
    },
    [1] = {
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,        BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,                      BLACK,
        BLACK, BLACK, BLACK,               BLACK,                      BLACK, BLACK, BLACK, BLACK,        BLACK, BLACK, BLACK
    },
    [2] = {
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,        CYAN,  CYAN,  CYAN,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, CYAN,  CYAN,  CYAN,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, CYAN,  CYAN,  CYAN,  CYAN,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, CYAN,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,                      BLACK,
        BLACK, BLACK, BLACK,               BLACK,                      BLACK, BLACK, BLACK, BLACK,        BLACK, BLACK, BLACK,
        // Boarder; starts bottom right and moves clockwise
        CYAN, CYAN, CYAN, DIM_RED, DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED,
        DIM_RED, DIM_WHITE, DIM_RED,
        DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED, DIM_RED, CYAN, CYAN, CYAN,
        CYAN, CYAN, CYAN
    },
    [3] = {
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,        BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,                      BLACK,
        BLACK, BLACK, BLACK,               BLACK,                      BLACK, BLACK, BLACK, BLACK,        BLACK, BLACK, BLACK,
        // Boarder; starts bottom right and moves clockwise
        DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED,
        DIM_RED, DIM_WHITE, DIM_RED,
        DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED,
        DIM_RED, DIM_WHITE, DIM_RED
    },
    [4] = {
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,        BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,                      BLACK,
        BLACK, BLACK, BLACK,               BLACK,                      BLACK, BLACK, BLACK, BLACK,        BLACK, BLACK, BLACK,
        // Boarder; starts bottom right and moves clockwise
        DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED,
        DIM_RED, DIM_WHITE, DIM_RED,
        DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED, DIM_RED, DIM_WHITE, DIM_RED, DIM_RED,
        DIM_RED, DIM_WHITE, DIM_RED
    }
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
    rgb_enabled_flag = true; // Initially, keyboard RGB is enabled. Change to false config.h initializes RGB disabled.
};

// Runs just one time after everything else has initialized.
void keyboard_post_init_user(void) {
    rgb_matrix_enable();
}

#define MODS_SHIFT  (get_mods() & MOD_MASK_SHIFT)
#define MODS_CTRL   (get_mods() & MOD_MASK_CTRL)
#define MODS_ALT    (get_mods() & MOD_MASK_ALT)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;

    switch (keycode) {
        case A_AC_LOW: // Lowercase a with acute accent: ALT + 0225
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_2) SS_TAP(X_KP_5)));
            }
            return false;
        case A_AC_CAP: // Uppercase A with acute accent: ALT + 0193
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_1) SS_TAP(X_KP_9) SS_TAP(X_KP_3)));
            }
            return false;
        case E_AC_LOW: // Lowercase e with acute accent: ALT + 0233
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_3) SS_TAP(X_KP_3)));
            }
            return false;
        case E_AC_CAP: // Uppercase E with acute accent: ALT + 0201
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_0) SS_TAP(X_KP_1)));
            }
            return false;
        case I_AC_LOW: // Lowercase i with acute accent: ALT + 0237
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_3) SS_TAP(X_KP_7)));
            }
            return false;
        case I_AC_CAP: // Uppercase I with acute accent: ALT + 0205
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_0) SS_TAP(X_KP_5)));
            }
            return false;
        case O_AC_LOW: // Lowercase o with acute accent: ALT + 0243
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_4) SS_TAP(X_KP_3)));
            }
            return false;
        case O_AC_CAP: // Uppercase O with acute accent: ALT + 0211
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_1) SS_TAP(X_KP_1)));
            }
            return false;
        case U_AC_LOW: // Lowercase u with acute accent: ALT + 0250
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_5) SS_TAP(X_KP_0)));
            }
            return false;
        case U_AC_CAP: // Uppercase U with acute accent: ALT + 0218
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_1) SS_TAP(X_KP_8)));
            }
            return false;
        case ENE_LOW: // Lowercase n with tilde accent: ALT + 0241
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_4) SS_TAP(X_KP_1)));
            }
            return false;
        case ENE_CAP: // Uppercase N with tilde accent: ALT + 0209
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_0) SS_TAP(X_KP_9)));
            }
            return false;
        case INV_EXC: // Inverted exclamation point: ALT + 161
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_1) SS_TAP(X_KP_6) SS_TAP(X_KP_1)));
            }
            return false;
        case INV_QUS: // Inverted question mark: ALT + 191
            if (record->event.pressed) {
                SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_1) SS_TAP(X_KP_9) SS_TAP(X_KP_1)));
            }
            return false;
        // DEFAULTS BELOW
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
        case RGB_TOG:
            if (record->event.pressed) {
              switch (rgb_matrix_get_flags()) {
                case LED_FLAG_ALL: {
                    rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case (LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR): {
                    rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case LED_FLAG_UNDERGLOW: {
                    rgb_matrix_set_flags(LED_FLAG_NONE);
                    rgb_matrix_disable_noeeprom();
                  }
                  break;
                default: {
                    rgb_matrix_set_flags(LED_FLAG_ALL);
                    rgb_matrix_enable_noeeprom();
                  }
                  break;
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
        } else if (layer != 1) {
            // Only deactivate non-defined key LEDs at layers other than FN. Because at FN we have RGB adjustments and need to see them live.
            // If the values are all false then it's a transparent key and deactivate LED at this layer
            rgb_matrix_set_color(i, 0, 0, 0);
        }
    }
}

bool rgb_matrix_indicators_user(void) {
    if (
        rgb_matrix_get_flags() == LED_FLAG_NONE ||
        rgb_matrix_get_flags() == LED_FLAG_UNDERGLOW) {
            return true;
        }
    set_layer_color(get_highest_layer(layer_state));
    return false;
}
