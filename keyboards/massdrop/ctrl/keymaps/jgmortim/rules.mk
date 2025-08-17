# RGBLIGHT_ENABLE = no          # Not for MD boards. This is here in case you forget.
TAP_DANCE_ENABLE = yes          # Enable tap dance keys
SRC += config_led.c             # Used to add files to the compilation/linking list.
SRC += windows_functions.c
SRC += linux_functions.c
