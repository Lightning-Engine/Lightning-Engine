#include "li/keymap.h"
#include <windows.h>

li_key_t li_win_xlat_key(int keycode) {
	switch (keycode) {
		case 0x001: return li_key_esc;
		case 0x03B: return li_key_f1;
		case 0x03C: return li_key_f2;
		case 0x03D: return li_key_f3;
		case 0x03E: return li_key_f4;
		case 0x03F: return li_key_f5;
		case 0x040: return li_key_f6;
		case 0x041: return li_key_f7;
		case 0x042: return li_key_f8;
		case 0x043: return li_key_f9;
		case 0x044: return li_key_f10;
		case 0x057: return li_key_f11;
		case 0x058: return li_key_f12;
		case 0x029: return li_key_tilde;
		case 0x00C: return li_key_minus;
		case 0x00D: return li_key_equal;
		case 0x00E: return li_key_backspace;
		case 0x00F: return li_key_tab;
		case 0x01A: return li_key_left_bracket;
		case 0x01B: return li_key_right_bracket;
		case 0x02B: return li_key_backslash;
		case 0x03A: return li_key_caps_lock;
		case 0x027: return li_key_semicolon;
		case 0x028: return li_key_apostrophe;
		case 0x01C: return li_key_enter;
		case 0x02A: return li_key_left_shift;
		case 0x033: return li_key_comma;
		case 0x034: return li_key_period;
		case 0x035: return li_key_slash;
		case 0x036: return li_key_right_shift;
		case 0x01D: return li_key_left_control;
		case 0x15B: return li_key_left_super;
		case 0x038: return li_key_left_alt;
		case 0x039: return li_key_space;
		case 0x138: return li_key_right_alt;
		case 0x15C: return li_key_right_menu;
		case 0x11D: return li_key_right_control;
		case 0x137: return li_key_print;
		case 0x046: return li_key_scroll_lock;
		case 0x045: return li_key_pause;
		case 0x146: return li_key_pause;
		case 0x152: return li_key_insert;
		case 0x147: return li_key_home;
		case 0x149: return li_key_page_up;
		case 0x153: return li_key_delete;
		case 0x14F: return li_key_end;
		case 0x151: return li_key_page_down;
		case 0x148: return li_key_up;
		case 0x150: return li_key_down;
		case 0x14B: return li_key_left;
		case 0x14D: return li_key_right;
		case 0x145: return li_key_num_lock;
		case 0x135: return li_key_num_divide;
		case 0x037: return li_key_num_multiply;
		case 0x04A: return li_key_num_minus;
		case 0x047: return li_key_num_7;
		case 0x048: return li_key_num_8;
		case 0x049: return li_key_num_9;
		case 0x04E: return li_key_num_plus;
		case 0x04B: return li_key_num_4;
		case 0x04C: return li_key_num_5;
		case 0x04D: return li_key_num_6;
		case 0x04F: return li_key_num_1;
		case 0x050: return li_key_num_2;
		case 0x051: return li_key_num_3;
		case 0x11C: return li_key_num_enter;
		case 0x052: return li_key_num_0;
		case 0x053: return li_key_num_period;
		case 0x002: return li_key_1;
		case 0x003: return li_key_2;
		case 0x004: return li_key_3;
		case 0x005: return li_key_4;
		case 0x006: return li_key_5;
		case 0x007: return li_key_6;
		case 0x008: return li_key_7;
		case 0x009: return li_key_8;
		case 0x00A: return li_key_9;
		case 0x00B: return li_key_0;
		case 0x01E: return li_key_char_a;
		case 0x030: return li_key_char_b;
		case 0x02E: return li_key_char_c;
		case 0x020: return li_key_char_d;
		case 0x012: return li_key_char_e;
		case 0x021: return li_key_char_f;
		case 0x022: return li_key_char_g;
		case 0x023: return li_key_char_h;
		case 0x017: return li_key_char_i;
		case 0x024: return li_key_char_j;
		case 0x025: return li_key_char_k;
		case 0x026: return li_key_char_l;
		case 0x032: return li_key_char_m;
		case 0x031: return li_key_char_n;
		case 0x018: return li_key_char_o;
		case 0x019: return li_key_char_p;
		case 0x010: return li_key_char_q;
		case 0x013: return li_key_char_r;
		case 0x01F: return li_key_char_s;
		case 0x014: return li_key_char_t;
		case 0x016: return li_key_char_u;
		case 0x02F: return li_key_char_v;
		case 0x011: return li_key_char_w;
		case 0x02D: return li_key_char_x;
		case 0x015: return li_key_char_y;
		case 0x02C: return li_key_char_z;
		default: return li_key_unknown;
	}
}

li_button_t li_win_xlat_button(int button) {
	switch (button) {
		case MK_LBUTTON: return li_button_left;
		case MK_MBUTTON: return li_button_middle;
		case MK_RBUTTON: return li_button_right;
		default: return li_button_unknown;
	}
}

li_key_state_t li_win_xlat_key_state(int state) {
	return state;
}