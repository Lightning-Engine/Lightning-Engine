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

int _windows_get_button_state(WPARAM w_param) {
	return (w_param);	
}

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	li_event_t event;

	memset(&event, 0, sizeof(li_event_t));
	event.data.button.button = -1;
	switch (msg) {
		case WM_DESTROY:
			event.type = li_event_close;
			event.window = (li_win_t) hwnd;
			li_win_cb(&event);
			PostQuitMessage(0);
			return 0;
		case WM_KEYUP:
		case WM_KEYDOWN:
			if (l_param & 0xF)
				event.type = li_event_key_repeat;
			else if (msg == WM_KEYUP)
				event.type = li_event_key_release;
			else
				event.type = li_event_key_press;
			event.data.key.key = w_param; /*Temporary, todo change to universal lightning keycode*/
			li_win_cb(&event);
			return 0;
		case WM_LBUTTONDOWN:
			event.data.button.button = _windows_translate_button(msg, w_param);
		case WM_MBUTTONDOWN:
			if (event.data.button.button < 0)
				event.data.button.button = _windows_translate_button(msg, w_param);
		case WM_RBUTTONDOWN:
			if (event.data.button.button < 0)
				event.data.button.button = _windows_translate_button(msg, w_param);
		case WM_XBUTTONDOWN:
			if (event.data.button.button < 0)
				event.data.button.button = _windows_translate_button(msg, w_param);
			event.type = li_event_button_press;
			event.data.button.state = _windows_get_button_state(w_param);
			li_win_cb(&event);
			return 0;
		case WM_LBUTTONUP:
			event.data.button.button = _windows_translate_button(msg, w_param);
		case WM_MBUTTONUP:
			if (event.data.button.button < 0)
				event.data.button.button = _windows_translate_button(msg, w_param);
		case WM_RBUTTONUP:
			if (event.data.button.button < 0)
				event.data.button.button = _windows_translate_button(msg, w_param);
		case WM_XBUTTONUP:
			if (event.data.button.button < 0)
				event.data.button.button = _windows_translate_button(msg, w_param);
			event.type = li_event_button_press;
			event.data.button.state = _windows_get_button_state(w_param);
			li_win_cb(&event);
			return 0;
		case WM_MOUSEMOVE:
			event.type = li_event_motion_notify;
			event.data.motion.x = LOWORD(l_param);
			event.data.motion.y = HIWORD(l_param);
			li_win_cb(&event);
			return 0;

	}
	return DefWindowProc(hwnd, msg, w_param, l_param);
}
