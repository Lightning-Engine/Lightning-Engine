#include "li/keymap.h"
#include "li/platform/xlib.h"
#include <X11/keysym.h>

static li_key_t xlat_keysym(KeySym sym) {
	switch (sym) {
		case XK_Escape: return li_key_esc;
		case XK_F1: return li_key_f1;
		case XK_F2: return li_key_f2;
		case XK_F3: return li_key_f3;
		case XK_F4: return li_key_f4;
		case XK_F5: return li_key_f5;
		case XK_F6: return li_key_f6;
		case XK_F7: return li_key_f7;
		case XK_F8: return li_key_f8;
		case XK_F9: return li_key_f9;
		case XK_F10: return li_key_f10;
		case XK_F11: return li_key_f11;
		case XK_F12: return li_key_f12;
		case XK_grave: return li_key_tilde;
		case XK_1: return li_key_1;
		case XK_2: return li_key_2;
		case XK_3: return li_key_3;
		case XK_4: return li_key_4;
		case XK_5: return li_key_5;
		case XK_6: return li_key_6;
		case XK_7: return li_key_7;
		case XK_8: return li_key_8;
		case XK_9: return li_key_9;
		case XK_0: return li_key_0;
		case XK_minus: return li_key_minus;
		case XK_equal: return li_key_equal;
		case XK_BackSpace: return li_key_backspace;
		case XK_Tab: return li_key_tab;
		case XK_Q: return li_key_char_q;
		case XK_W: return li_key_char_w;
		case XK_E: return li_key_char_e;
		case XK_R: return li_key_char_r;
		case XK_T: return li_key_char_t;
		case XK_Y: return li_key_char_y;
		case XK_U: return li_key_char_u;
		case XK_I: return li_key_char_i;
		case XK_O: return li_key_char_o;
		case XK_P: return li_key_char_p;
		case XK_bracketleft: return li_key_left_bracket;
		case XK_bracketright: return li_key_right_bracket;
		case XK_backslash: return li_key_backslash;
		case XK_Caps_Lock: return li_key_caps_lock;
		case XK_A: return li_key_char_a;
		case XK_S: return li_key_char_s;
		case XK_D: return li_key_char_d;
		case XK_F: return li_key_char_f;
		case XK_G: return li_key_char_g;
		case XK_H: return li_key_char_h;
		case XK_J: return li_key_char_j;
		case XK_K: return li_key_char_k;
		case XK_L: return li_key_char_l;
		case XK_semicolon: return li_key_semicolon;
		case XK_apostrophe: return li_key_apostrophe;
		case XK_Return: return li_key_enter;
		case XK_Shift_L: return li_key_left_shift;
		case XK_Z: return li_key_char_z;
		case XK_X: return li_key_char_x;
		case XK_C: return li_key_char_c;
		case XK_V: return li_key_char_v;
		case XK_B: return li_key_char_b;
		case XK_N: return li_key_char_n;
		case XK_M: return li_key_char_m;
		case XK_comma: return li_key_comma;
		case XK_period: return li_key_period;
		case XK_slash: return li_key_slash;
		case XK_Shift_R: return li_key_right_shift;
		case XK_Control_L: return li_key_left_control;
		case XK_Super_L: return li_key_windows;
		case XK_Alt_L: return li_key_left_alt;
		case XK_space: return li_key_space;
		case XK_Alt_R: return li_key_right_alt;
		case XK_Menu: return li_key_select;
		case XK_Control_R: return li_key_right_control;
		case XK_Print: return li_key_print;
		case XK_Scroll_Lock: return li_key_scroll;
		case XK_Pause: return li_key_pause;
		case XK_Insert: return li_key_insert;
		case XK_Home: return li_key_home;
		case XK_Page_Up: return li_key_page_up;
		case XK_Delete: return li_key_delete;
		case XK_End: return li_key_end;
		case XK_Page_Down: return li_key_page_down;
		case XK_Up: return li_key_up;
		case XK_Down: return li_key_down;
		case XK_Left: return li_key_left;
		case XK_Right: return li_key_right;
		case XK_Num_Lock: return li_key_num_lock;
		case XK_KP_Divide: return li_key_num_divide;
		case XK_KP_Multiply: return li_key_num_multiply;
		case XK_KP_Subtract: return li_key_num_minus;
		case XK_KP_7: return li_key_num_7;
		case XK_KP_8: return li_key_num_8;
		case XK_KP_9: return li_key_num_9;
		case XK_KP_Add: return li_key_num_plus;
		case XK_KP_4: return li_key_num_4;
		case XK_KP_5: return li_key_num_5;
		case XK_KP_6: return li_key_num_6;
		case XK_KP_1: return li_key_num_1;
		case XK_KP_2: return li_key_num_2;
		case XK_KP_3: return li_key_num_3;
		case XK_KP_Enter: return li_key_num_enter;
		case XK_KP_0: return li_key_num_0;
		case XK_KP_Decimal: return li_key_num_period;
		default: return li_key_unknown;
	}
}

li_key_t li_win_xlat_key(int keycode)
{
	int count;
	li_key_t key = li_key_unknown;
	KeySym *keysym = XGetKeyboardMapping(li_xlib_display, keycode, 1, &count);
	for (int i = 0; i < count && keysym[i] != NoSymbol && key == li_key_unknown; i++)
		key = xlat_keysym(keysym[i]);
	XFree(keysym);
	return key;
}

li_button_t li_win_xlat_button(int button)
{
	switch (button) {
		case 1: return li_button_left;
		case 2: return li_button_middle;
		case 3: return li_button_right;
		default: return li_button_unknown;
	}
}