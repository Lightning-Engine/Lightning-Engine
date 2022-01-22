#include "li/win.h"
#include <windows.h>

const static char *LI_DEFAULT_CLASS_NAME = "LIWINDOW";
static HINSTANCE LI_WIN_HANDLE;

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

int li_win_init(void) {
	WNDCLASS wndClass = { };

	LI_WIN_HANDLE = GetModuleHandleA(NULL);
	wndClass.hInstance = LI_WIN_HANDLE;
	wndClass.lpszClassName = LI_DEFAULT_CLASS_NAME;
	wndClass.lpfnWndProc = winProc;

	if (RegisterClass(&wndClass) == 0)
		return -1;
}

void li_win_exit(void) {

}

void li_win_poll(void) {
	MSG msg;
	while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

int li_win_create(li_win_t *win, int width, int height) {
	win->p = CreateWindowExA(0, LI_DEFAULT_CLASS_NAME, "New Window", 
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		width, height, NULL, NULL, LI_WIN_HANDLE, NULL);
	if (win->p == 0)
		return -1;
}


void li_win_destroy(li_win_t win) {
	DestroyWindow(win.p);
}

void li_win_map(li_win_t win) {
	ShowWindow(win.p, SW_SHOW);
}

int _windows_translate_button(UINT button_event, WPARAM w_param) {
	switch (button_event) {
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			return MK_LBUTTON;
		case WM_MBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
			return MK_MBUTTON;
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
			return MK_RBUTTON;
		case WM_XBUTTONUP:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONDBLCLK:
			if (HIWORD(w_param) == XBUTTON1)
				return MK_XBUTTON1;
			return MK_XBUTTON2;
	}
	return 0;
}

int _windows_get_key_state(void) {
	int	state;

	state = 0;
	state |= li_key_state_shift * (GetKeyState(VK_SHIFT) != 0);
	state |= li_key_state_control * (GetKeyState(VK_CONTROL) != 0);
	state |= li_key_state_alt * (GetKeyState(VK_MENU) != 0);
	state |= li_key_state_num_lock * (GetKeyState(VK_NUMLOCK) & 0b1);
	state |= li_key_state_caps_lock * (GetKeyState(VK_CAPITAL) & 0b1);
	state |= li_key_state_scroll_lock * (GetKeyState(VK_SCROLL) & 0b1);
}

int _windows_get_button_state(WPARAM w_param) {
	int state;

	state = _windows_get_key_state();
	state |= li_button_state_left * ((w_param & MK_LBUTTON) != 0);
	state |= li_button_state_middle * ((w_param & MK_MBUTTON) != 0);
	state |= li_button_state_right * ((w_param & MK_RBUTTON) != 0);
	return (state);	
}

/*Also gets if it's an extended key*/
short _windows_get_scancode(WPARAM w_param) {
	return ((short)(w_param & 0x1FF0000) >> 16);
}


int _windows_handle_key_event(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	li_event_t event;

	event.any.window = ((li_win_t) ((void*) hwnd));
	event.key.key =  li_win_xlat_key(_windows_get_scancode(w_param));
	event.key.state = li_win_xlat_key_state(_windows_get_key_state());
	switch (msg) {
		case WM_KEYDOWN:
			if (l_param & (1 << 30))
				event.any.type = li_event_key_repeat;
			else
				event.any.type = li_event_key_press;
			li_win_cb(&event);
			return 0;
		case WM_KEYUP:
			event.any.type = li_event_key_release;
			li_win_cb(&event);
			return 0;
		default:
			return 1;
	}
}

int _windows_handle_mouse_event(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	li_event_t event;

	event.any.window = ((li_win_t) ((void*) hwnd));
	event.button.button = li_win_xlat_button(_windows_translate_button(msg, w_param));
	event.button.state = li_win_xlat_key_state(_windows_get_button_state(w_param));
	event.button.x = LOWORD(l_param);
	event.button.y = HIWORD(l_param);
	switch (msg) {
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_XBUTTONDOWN:
			event.any.type = li_event_button_press;
			li_win_cb(&event);
			return 0;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_XBUTTONUP:
			event.any.type = li_event_button_release;
			li_win_cb(&event);
			return 0;
		case WM_MOUSEMOVE:
			event.any.type = li_event_motion_notify;
			li_win_cb(&event);
			return 0;
		default:
			return 1;
	}
}

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	li_event_t event;

	switch (msg) {
		case WM_DESTROY:
			event.any.type = li_event_close;
			event.any.window = ((li_win_t) ((void*) hwnd));
			li_win_cb(&event);
			PostQuitMessage(0);
			return 0;
		case WM_KEYUP:
		case WM_KEYDOWN:
			_windows_handle_key_event(hwnd, msg, w_param, l_param);
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_XBUTTONUP:
		case WM_MOUSEMOVE:
			_windows_handle_mouse_event(hwnd, msg, w_param, l_param);

	}
	return DefWindowProc(hwnd, msg, w_param, l_param);
}
