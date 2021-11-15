#include <windows.h>

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prev_instance, PWSTR args, int cmd_show) {
	const wchar_t class_name[] = L"TEST_CLASS";

	WNDCLASS wnd_class = { };

	wnd_class.hInstance = instance;
	wnd_class.lpszClassName = class_name;
	wnd_class.lpfnWndProc = winProc;

	RegisterClass(&wnd_class);
	HWND hwnd = CreateWindowEx(0, class_name, L"Hello World", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, instance, NULL);
	if (hwnd == NULL) {
		//Something went wrong
		return (1);
	}

	ShowWindow(hwnd, cmd_show);

	MSG msg;
	while (1) {
		while (PeekMessageW(&msg, hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
}

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, w_param, l_param);
}