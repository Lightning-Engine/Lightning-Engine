#include "li/win.h"
#include <windows.h>

static const wchar_t *LI_DEFAULT_CLASS_NAME = L"LIWINDOW";
static HINSTANCE LI_WIN_HANDLE;
static void (*li_win_cb)(li_event_t*);

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

int li_win_init(void (*cb)(li_event_t*)) {
	WNDCLASS wndClass = { 0 };
	LI_WIN_HANDLE = GetModuleHandleA(NULL);
	wndClass.hInstance = LI_WIN_HANDLE;
	wndClass.lpszClassName = LI_DEFAULT_CLASS_NAME;
	wndClass.lpfnWndProc = winProc;
	li_win_cb = cb;

	if (RegisterClass(&wndClass) == 0)
		return -1;
	return 0;
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
	win->p = CreateWindowExW(0, LI_DEFAULT_CLASS_NAME, L"New Window", 
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		width, height, NULL, NULL, LI_WIN_HANDLE, NULL);
	if (win->p == 0)
		return -1;
	return 0;
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
	state |= li_key_state_shift * !!(GetKeyState(VK_SHIFT) & 0x8000);
	state |= li_key_state_control * !!(GetKeyState(VK_CONTROL) & 0x8000);
	state |= li_key_state_alt * !!(GetKeyState(VK_MENU) & 0x8000);
	state |= li_key_state_num_lock * (GetKeyState(VK_NUMLOCK) & 0x1);
	state |= li_key_state_caps_lock * (GetKeyState(VK_CAPITAL) & 0x1);
	state |= li_key_state_scroll_lock * (GetKeyState(VK_SCROLL) & 0x1);
	state |= li_button_state_left * !!(GetKeyState(VK_LBUTTON) & 0x8000);
	state |= li_button_state_middle * !!(GetKeyState(VK_MBUTTON) & 0x8000);
	state |= li_button_state_right * !!(GetKeyState(VK_RBUTTON) & 0x8000);
	return state;
}

/*Also gets if it's an extended key*/
short _windows_get_scancode(LPARAM l_param) {
	return ((l_param & 0x1FF0000) >> 16);
}


int _windows_handle_key_event(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	li_event_t event;

	event.any.window.p = hwnd;
	event.key.key =  li_win_xlat_key(_windows_get_scancode(l_param));
	event.key.state = li_win_xlat_key_state(_windows_get_key_state());
	(void)w_param;
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
	return 1;
}

int _windows_handle_mouse_event(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	li_event_t event;

	event.any.window.p = hwnd;
	switch (msg) {
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_XBUTTONDOWN:
			event.button.button = li_win_xlat_button(_windows_translate_button(msg, w_param));
			event.button.state = li_win_xlat_key_state(_windows_get_key_state());
			event.button.x = LOWORD(l_param);
			event.button.y = HIWORD(l_param);
			event.any.type = li_event_button_press;
			li_win_cb(&event);
			return 0;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_XBUTTONUP:
			event.button.button = li_win_xlat_button(_windows_translate_button(msg, w_param));
			event.button.state = li_win_xlat_key_state(_windows_get_key_state());
			event.button.x = LOWORD(l_param);
			event.button.y = HIWORD(l_param);
			event.any.type = li_event_button_release;
			li_win_cb(&event);
			return 0;
		case WM_MOUSEMOVE:
			event.motion.state = li_win_xlat_key_state(_windows_get_key_state());
			event.motion.x = LOWORD(l_param);
			event.motion.y = HIWORD(l_param);
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
			event.any.window.p = hwnd;
			li_win_cb(&event);
			PostQuitMessage(0);
			return 0;
		case WM_KEYUP:
		case WM_KEYDOWN:
			if (!_windows_handle_key_event(hwnd, msg, w_param, l_param))
				return 0;
			break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_XBUTTONUP:
		case WM_MOUSEMOVE:
			if (!_windows_handle_mouse_event(hwnd, msg, w_param, l_param))
				return 0;
			break;

	}
	return DefWindowProc(hwnd, msg, w_param, l_param);
}
