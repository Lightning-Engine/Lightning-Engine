#include "li/win.h"
#include "li/assert.h"
#include "li/dl.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/wgl.h>

static const wchar_t *LI_DEFAULT_CLASS_NAME = L"LIWINDOW";
static HINSTANCE LI_WIN_HANDLE;
static void (*li_win_cb)(li_event_t*);
static li_dl_t opengl32 = { .p = NULL };

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);


HDC _windows_get_hdc(li_win_t win) {
	LONG_PTR ptr = GetWindowLongPtrW(win.p, GWLP_USERDATA);
	if (ptr == 0) {
		ptr = (LONG_PTR) GetDC(win.p);
		SetWindowLongPtrW(win.p, GWLP_USERDATA, (LONG_PTR) ptr);
		
	}
	return (HDC) ptr;
}

void _windows_release_hdcs(li_win_t win) {
	HDC hdc;

	hdc = _windows_get_hdc(win);
	li_assert(hdc != 0);
	ReleaseDC(win.p, hdc);
}


void li_win_init(void (*cb)(li_event_t*)) {
	WNDCLASSW wndClass = { 0 };
	LI_WIN_HANDLE = GetModuleHandleA(NULL);
	wndClass.hInstance = LI_WIN_HANDLE;
	wndClass.lpszClassName = LI_DEFAULT_CLASS_NAME;
	wndClass.lpfnWndProc = winProc;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	li_win_cb = cb;
	li_assert(RegisterClassW(&wndClass) != 0);
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

li_win_t li_win_create(int width, int height) {
	li_win_t win;
	win.p = CreateWindowExW(CS_OWNDC, LI_DEFAULT_CLASS_NAME, L"New Window", 
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		width, height, NULL, NULL, LI_WIN_HANDLE, NULL);
	li_assert(win.p != 0);
	return win;
}


void li_win_destroy(li_win_t win) {
	_windows_release_hdcs(win);
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
	state |= li_key_state_super * !!((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000);
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
		case WM_SYSKEYDOWN:
			if (l_param & (1 << 30))
				event.any.type = li_event_key_repeat;
			else
				event.any.type = li_event_key_press;
			li_win_cb(&event);
			return msg == WM_SYSKEYDOWN;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			event.any.type = li_event_key_release;
			li_win_cb(&event);
			return msg == WM_SYSKEYUP;
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

int _windows_handle_resize_event(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	li_event_t event;

	(void)w_param;
	(void)msg;
	event.resize.any.window.p = hwnd;
	event.resize.any.type = li_event_window_resize;
	event.resize.width = LOWORD(l_param);
	event.resize.height = HIWORD(l_param);
	li_win_cb(&event);
	return 0;
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
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
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
		case WM_SIZE:
			if (!_windows_handle_resize_event(hwnd, msg, w_param, l_param))
				return 0;
			break;

	}
	return DefWindowProc(hwnd, msg, w_param, l_param);
}

PIXELFORMATDESCRIPTOR _windows_get_pfd(void) {
	PIXELFORMATDESCRIPTOR pfd;

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	return pfd;
}

li_ctx_t li_ctx_create(li_win_t win, int version) {
	(void) version;

	PIXELFORMATDESCRIPTOR pfd;
	li_ctx_t dummy_context;
	li_ctx_t actual_context;
	int pixelFormat;
	HDC hdc;

	hdc = _windows_get_hdc(win);
	pfd = _windows_get_pfd();
	pixelFormat = ChoosePixelFormat(hdc, &pfd);
	li_assert(pixelFormat != 0);
	li_assert(SetPixelFormat(hdc, pixelFormat, &pfd));

	dummy_context.p = (void*) wglCreateContext(hdc);
	li_assert(dummy_context.p != NULL);
	li_assert(wglMakeCurrent(hdc, dummy_context.p));
	
	int contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		/*WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,*/
		0
	};

	PROC proc = wglGetProcAddress("wglCreateContextAttribsARB");
	PFNWGLCREATECONTEXTATTRIBSARBPROC func = *(PFNWGLCREATECONTEXTATTRIBSARBPROC*) &proc; 
	actual_context.p = func(hdc, NULL, contextAttribs);
	li_assert(wglDeleteContext(dummy_context.p));
	
	return actual_context;
}

void li_ctx_destroy(li_win_t win, li_ctx_t ctx) {
	(void) win;
	li_assert(wglDeleteContext(ctx.p));
}

void li_ctx_make_current(li_win_t win, li_ctx_t ctx) {
	li_assert(wglMakeCurrent(_windows_get_hdc(win), ctx.p));
}

void li_ctx_swap_buffers(li_win_t win, li_ctx_t ctx) {
	(void) ctx;
	SwapBuffers(_windows_get_hdc(win));
}

void *li_ctx_get_proc_addr(const char *name) {
	if (opengl32.p == NULL)
		opengl32 = li_dl_open("opengl32.dll");
	li_assert(opengl32.p != NULL);
	PROC fun = wglGetProcAddress(name);
	void *ptr = *(void**) &fun;
	// https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions
	// > While the MSDN documentation says that wglGetProcAddress returns NULL on failure,
	// > some implementations will return other values. 1, 2, and 3 are used, as well as -1. 
	if (ptr == (void*) 0 || ptr == (void*) 1 || ptr == (void*) 2 || ptr == (void*) 3 || ptr == (void*) -1)
		ptr = li_dl_sym(opengl32, name);
	return ptr;
}
