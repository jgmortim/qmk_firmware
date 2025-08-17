#pragma once

#define MILLISECONDS_IN_SECOND 1000

#define EDGE_LED {12, 255, 100}   // Default color for edge LEDs.
#define KEY_LED {12, 255, 225}    // Default LED color for backlit keys.
#define SP_IND {HSV_CYAN}         // Spanish layer indicator LED color.
#define WIN_IND {HSV_SPRINGGREEN} // Windows mode indicator LED color.
#define LNX_IND {HSV_PURPLE}      // Linux mode indicator LED color.
#define CAPS_IND {12, 255, 225}   // Caps Lock indicator LED color.
#define SCRL_IND {12, 255, 225}   // Scroll Lock indicator LED color.
#define ______ {HSV_OFF}          // 5 underscores instead of the 6 used by the KC_TRNS alias.

#define RGB_TIME_OUT 300       // 300 seconds (5 minutes).
#define OS_MODE_IND_TIME_OUT 3 // 3 second timeout for the OS mode indicator LED.

#define SCRL_LOCK_IND_LED 14   // Index of the Scroll Lock indicator LED (14 is the Scroll Lock key).
#define CAPS_LOCK_IND_LED 50   // Index of the Caps Lock indicator LED (50 is the Caps Lock key).
#define OS_MODE_IND_LED 77     // Index of the OS mode indicator LED (77 is Win key).

#define MODS_SHIFT  (get_mods() & MOD_MASK_SHIFT)
#define MODS_CTRL   (get_mods() & MOD_MASK_CTRL)
#define MODS_ALT    (get_mods() & MOD_MASK_ALT)


/* windows_functions.c */
void windows_run(const char *command);
void send_alt_code(const char code[]);

/* linux_functions.c */
void linux_run(const char *command);
void send_unicode(const char *code);
