#include "li/keymap.h"

li_key_t li_win_xlat_key(int keycode) {
    switch (keycode) {
        default:
            return keycode;
    }
}

li_button_t li_win_xlat_button(int button) {
    switch (button) {
        default:
            return button;
    }
}

li_key_state_t li_win_xlat_key_state(int state) {
    (void)state;
    return 0;
}
