#include "li/keymap.h"
#include <windows.h>

li_key_t li_win_xlat_key(int keycode) {
	switch (keycode) {
		case VK_BACK:
			return li_key_unknown;
		case VK_TAB:
			return li_key_tab;
		case VK_CLEAR:
			return li_key_unknown;
		case VK_RETURN:
			return li_key_enter;
		case VK_SHIFT: /*Welke shift?*/
			return li_key_left_shift;
		case VK_CONTROL: /*Welke ctrl?*/
			return li_key_left_control;
		case VK_MENU: /*Welke alt?*/
			return li_key_left_alt;
		case VK_PAUSE:
			return li_key_pause;
		case VK_CAPITAL:
			return li_key_caps_lock;
		case VK_ESCAPE:
			return li_key_esc;
		case VK_SPACE:
			return li_key_space;
		case VK_PRIOR:
			return li_key_page_up;
		case VK_NEXT:
			return li_key_page_down;
		case VK_END:
			return li_key_end;
		case VK_HOME:
			return li_key_home;
		case VK_LEFT:
			return li_key_left;
		case VK_UP:
			return li_key_up;
		case VK_RIGHT:
			return li_key_right;
		case VK_DOWN:
			return li_key_down;
		case VK_SELECT:
			return li_key_select;
		case VK_SNAPSHOT:
			return li_key_print;
		case VK_INSERT:
			return li_key_insert;
		case VK_DELETE:
			return li_key_delete;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
			return keycode;
	}
}

li_button_t li_win_xlat_button(int button) {

}
