#include "li/entry.h"
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return -1;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prev_instance, PWSTR args, int cmd_show) {
	return li_main();
}