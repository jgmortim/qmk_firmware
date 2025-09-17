/*
Windows-Specific Helper Functions (Massdrop CTRL - jgmortim Keymap)
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

// Mapping for keypad digits.
static const char *const numpad_key_taps[] = {
    SS_TAP(X_KP_0), SS_TAP(X_KP_1), SS_TAP(X_KP_2), SS_TAP(X_KP_3), SS_TAP(X_KP_4),
    SS_TAP(X_KP_5), SS_TAP(X_KP_6), SS_TAP(X_KP_7), SS_TAP(X_KP_8), SS_TAP(X_KP_9)
};

/* Opens the Windows Run dialog and enters the given command. */
void windows_run(const char *command) {
    if (!command || !*command) return;      // Skip if input is empty.

    SEND_STRING(SS_LGUI("r") SS_DELAY(50)); // Open the run dialog,
    SEND_STRING(command);                   // type in the command,
    SEND_STRING(SS_TAP(X_ENT));             // and press enter.
}

/* Executes the given alt code. */
void send_alt_code(const char *code) {
    if (!code || !*code) return; // Skip if input is empty

    // Save current modifier state and disable them so that they don't interfere with execution.
    uint8_t saved_mods = get_mods();
    unregister_mods(saved_mods);

    SEND_STRING(SS_DOWN(X_LALT));
    for(int i = 0; code[i] != '\0'; i++) {
        char c = code[i];
        if (c >= '0' && c <= '9') {
            SEND_STRING(numpad_key_taps[c - '0']);
        }
    }
    SEND_STRING(SS_UP(X_LALT));

    register_mods(saved_mods); // Restores the modifier to their original state.
}
