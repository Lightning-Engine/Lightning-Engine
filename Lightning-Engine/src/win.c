#include "li/win.h"
#include "li/dl.h"

#define LI_LOAD_SYMBOL(name) { void* fn = li_dl_sym(lib, #name); if ((_##name = *(name##_type*) &fn) == 0) return -1; }

static li_dl_t lib;
typedef int (*li_win_init_type)(void (*cb)(li_event_t*));
typedef void (*li_win_exit_type)(void);
typedef void (*li_win_poll_type)(void);
typedef int (*li_win_create_type)(li_win_t*, int, int);
typedef void (*li_win_destroy_type)(li_win_t);
typedef void (*li_win_map_type)(li_win_t);

static li_win_init_type _li_win_init;
static li_win_exit_type _li_win_exit;
static li_win_poll_type _li_win_poll;
static li_win_create_type _li_win_create;
static li_win_destroy_type _li_win_destroy;
static li_win_map_type _li_win_map;

li_dl_t _li_open(void);

#ifdef LI_PLATFORM_POSIX
li_dl_t _li_open(void) {
	return li_dl_open("./libliengine_win.so");
}
#elif LI_PLATFORM_WINDOWS
li_dl_t _li_open(void) {
	return li_dl_open("liengine_win.dll");
}
#elif LI_PLATFORM_OSX
li_dl_t _li_open(void) {
	return li_dl_open("libliengine_win.so");
}
#else
#error unsupported platform
#endif

int li_win_init(void (*cb)(li_event_t*)) {
	lib = _li_open();
	if (lib.p == 0)
		return -1;

	printf("test\n");

	LI_LOAD_SYMBOL(li_win_init)
	LI_LOAD_SYMBOL(li_win_exit)
	LI_LOAD_SYMBOL(li_win_poll)
	LI_LOAD_SYMBOL(li_win_create)
	LI_LOAD_SYMBOL(li_win_destroy)
	LI_LOAD_SYMBOL(li_win_map)
	return _li_win_init(cb);
}

void li_win_exit(void) {
	_li_win_exit();
}

void li_win_poll(void) {
	_li_win_poll();
}

int li_win_create(li_win_t *win, int width, int height) {
	return _li_win_create(win, width, height);
}

void li_win_destroy(li_win_t win) {
	_li_win_destroy(win);
}

void li_win_map(li_win_t win) {
	_li_win_map(win);
}
