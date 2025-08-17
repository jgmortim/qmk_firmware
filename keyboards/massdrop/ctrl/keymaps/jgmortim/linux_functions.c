/*
Linux-Specific Helper Functions (Massdrop CTRL - jgmortim Keymap)
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

/* Enters the given command into the Linux Run dialog. */
void linux_run(const char *command) {
    if (!command || !*command) return;               // Skip if input is empty.

    // Save current modifier state and disable them so that they don't interfere with execution.
    uint8_t saved_mods = get_mods();
    unregister_mods(saved_mods);

    SEND_STRING(SS_LALT(SS_TAP(X_F2)) SS_DELAY(50)); // Open the run dialog,
    SEND_STRING(command);                            // type in the command,
    SEND_STRING(SS_TAP(X_ENT));                      // and press enter.

    register_mods(saved_mods);                       // Restores the mods to their original state.
}

/* Inserts the given Unicode character via Unicode-entry mode. */
void send_unicode(const char *code) {
     if (!code || !*code) return;       // Skip if input is empty.

    // Save current modifier state and disable them so that they don't interfere with execution.
    uint8_t saved_mods = get_mods();
    unregister_mods(saved_mods);

    SEND_STRING(SS_LCTL(SS_LSFT("u"))); // Ctrl + Shift  + U,
    SEND_STRING(code);                  // type in the code,
    SEND_STRING(SS_TAP(X_ENT));         // and press enter.

    register_mods(saved_mods);          // Restores the mods to their original state.
}
