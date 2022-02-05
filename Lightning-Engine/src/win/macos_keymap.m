#import "Cocoa/Cocoa.h"
#include "li/keymap.h"

li_key_t li_win_xlat_key(int keycode) {
	switch (keycode) {
		case 0: return (li_key_char_a);
		case 1: return (li_key_char_s);
		case 2: return (li_key_char_d);
		case 3: return (li_key_char_f);
		case 4: return (li_key_char_h);
		case 5: return (li_key_char_g);
		case 6: return (li_key_char_z);
		case 7: return (li_key_char_x);
		case 8: return (li_key_char_c);
		case 9: return (li_key_char_v);
		case 11: return (li_key_char_b);
		case 12: return (li_key_char_q);
		case 13: return (li_key_char_w);
		case 14: return (li_key_char_e);
		case 15: return (li_key_char_r);
		case 16: return (li_key_char_y);
		case 17: return (li_key_char_r);
		case 18: return (li_key_1);
		case 19: return (li_key_2);
		case 20: return (li_key_3);
		case 21: return (li_key_4);
		case 22: return (li_key_6);
		case 23: return (li_key_5);
		case 24: return (li_key_equal);
		case 25: return (li_key_9);
		case 26: return (li_key_7);
		case 27: return (li_key_minus);
		case 28: return (li_key_8);
		case 29: return (li_key_0);
		case 30: return (li_key_right_bracket);
		case 31: return (li_key_char_o);
		case 32: return (li_key_char_u);
		case 33: return (li_key_left_bracket);
		case 34: return (li_key_char_i);
		case 35: return (li_key_char_p); //
		case 36: return (li_key_enter);
		case 37: return (li_key_char_l);
		case 38: return (li_key_char_j);
		case 39: return (li_key_apostrophe);
		case 40: return (li_key_char_k);
		case 41: return (li_key_semicolon);
		case 42: return (li_key_slash);
		case 43: return (li_key_comma);
		case 44: return (li_key_backslash);
		case 45: return (li_key_char_n);
		case 46: return (li_key_char_m);
		case 47: return (li_key_period);
		case 48: return (li_key_tab);
		case 50: return (li_key_tilde);
		case 51: return (li_key_delete);
		case 53: return (li_key_esc);
		case 55: return (li_key_left_super); //todo check left or right
		case 56: return (li_key_left_shift); //todo check left or right
		case 57: return (li_key_caps_lock);
		case 58: return (li_key_left_alt); //todo check left or right
		case 59: return (li_key_left_control); //todo check left or right
		case 65: return (li_key_num_period);
		case 67: return (li_key_num_multiply);
		case 69: return (li_key_num_plus);
		case 71: return (li_key_num_lock);
		case 75: return (li_key_num_divide);
		case 76: return (li_key_num_enter);
		case 78: return (li_key_num_minus);
		case 82: return (li_key_num_0);
		case 83: return (li_key_num_1);
		case 84: return (li_key_num_2);
		case 85: return (li_key_num_3);
		case 86: return (li_key_num_4);
		case 87: return (li_key_num_5);
		case 88: return (li_key_num_6);
		case 89: return (li_key_num_7);
		case 91: return (li_key_num_8);
		case 92: return (li_key_num_9);
		case 96: return (li_key_f5);
		case 97: return (li_key_f6);
		case 98: return (li_key_f7);
		case 99: return (li_key_f3);
		case 100: return (li_key_f8);
		case 101: return (li_key_f9);
		case 103: return (li_key_f11);
		case 109: return (li_key_f10);
		case 111: return (li_key_f12);
		case 114: return (li_key_insert);
		case 115: return (li_key_home);
		case 116: return (li_key_page_up);
		case 117: return (li_key_delete);
		case 118: return (li_key_f4);
		case 119: return (li_key_end);
		case 121: return (li_key_page_down);
		case 122: return (li_key_f1);
		case 120: return (li_key_f2);
		case 123: return (li_key_left);
		case 124: return (li_key_right);
		case 125: return (li_key_down);
		case 126: return (li_key_up);
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
	li_key_state_t keyState = 0;
	if (state & NSEventModifierFlagCapsLock)
		keyState |= li_key_state_caps_lock;
	if (state & NSEventModifierFlagShift)
		keyState |= li_key_state_shift;
	if (state & NSEventModifierFlagControl)
		keyState |= li_key_state_control;
	if (state & NSEventModifierFlagOption)
		keyState |= li_key_state_alt;
	if (state & NSEventModifierFlagCommand)
		keyState |= li_key_state_super;
	return keyState;
}
