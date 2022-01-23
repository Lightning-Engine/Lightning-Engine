#include "li/win.h"
#include "li/dl.h"
#include "li/assert.h"
#include <stdlib.h>

#if 1 // enable symbol loading
#define LI_LOAD_SYMBOL(name) { void* fn = li_dl_sym(lib, #name); li_assert((_##name = *(name##_type*) &fn) != 0); }
#else
#define LI_LOAD_SYMBOL(name) 
#endif

#ifdef LI_PLATFORM_POSIX
#define LI_WIN_LIB "libliengine_win.so"
#elif LI_PLATFORM_WINDOWS
#define LI_WIN_LIB "liengine_win.dll"
#elif LI_PLATFORM_OSX
#define LI_WIN_LIB "libliengine_win.so"
#else
#error unsupported platform
#endif

static li_dl_t lib;
static int win_loaded = 0;
typedef void (*li_win_init_type)(void (*)(li_event_t*));
typedef void (*li_win_exit_type)(void);
typedef void (*li_win_poll_type)(void);
typedef li_win_t (*li_win_create_type)(int, int);
typedef void (*li_win_destroy_type)(li_win_t);
typedef void (*li_win_map_type)(li_win_t);
typedef li_ctx_t (*li_ctx_create_type)(li_win_t);
typedef void (*li_ctx_destroy_type)(li_ctx_t);
typedef void (*li_ctx_make_current_type)(li_win_t, li_ctx_t);
typedef void (*li_ctx_swap_buffers_type)(li_win_t);
typedef void* (*li_ctx_get_proc_addr_type)(const char*);

static li_win_init_type _li_win_init;
static li_win_exit_type _li_win_exit;
static li_win_poll_type _li_win_poll;
static li_win_create_type _li_win_create;
static li_win_destroy_type _li_win_destroy;
static li_win_map_type _li_win_map;
static li_ctx_create_type _li_ctx_create;
static li_ctx_destroy_type _li_ctx_destroy;
static li_ctx_make_current_type _li_ctx_make_current;
static li_ctx_swap_buffers_type _li_ctx_swap_buffers;
static li_ctx_get_proc_addr_type _li_ctx_get_proc_addr;

void li_win_load() {
	if (!win_loaded) {
		lib = li_dl_open_rel(LI_WIN_LIB);
		li_assert(lib.p != NULL);

		LI_LOAD_SYMBOL(li_win_init)
		LI_LOAD_SYMBOL(li_win_exit)
		LI_LOAD_SYMBOL(li_win_poll)
		LI_LOAD_SYMBOL(li_win_create)
		LI_LOAD_SYMBOL(li_win_destroy)
		LI_LOAD_SYMBOL(li_win_map)
		LI_LOAD_SYMBOL(li_ctx_create)
		LI_LOAD_SYMBOL(li_ctx_destroy)
		LI_LOAD_SYMBOL(li_ctx_make_current)
		LI_LOAD_SYMBOL(li_ctx_swap_buffers)
		LI_LOAD_SYMBOL(li_ctx_get_proc_addr)
		win_loaded = 1;
	}
}

void li_win_init(void (*cb)(li_event_t*)) {
	li_win_load();
	_li_win_init(cb);
}

void li_win_exit(void) {
	li_win_load();
	_li_win_exit();
}

void li_win_poll(void) {
	li_win_load();
	_li_win_poll();
}

li_win_t li_win_create(int width, int height) {
	li_win_load();
	return _li_win_create(width, height);
}

void li_win_destroy(li_win_t win) {
	li_win_load();
	_li_win_destroy(win);
}

void li_win_map(li_win_t win) {
	li_win_load();
	_li_win_map(win);
}

li_ctx_t li_ctx_create(li_win_t win)
{
	li_win_load();
	return _li_ctx_create(win);
}

void li_ctx_destroy(li_ctx_t ctx)
{
	li_win_load();
	_li_ctx_destroy(ctx);
}

void li_ctx_make_current(li_win_t win, li_ctx_t ctx)
{
	li_win_load();
	_li_ctx_make_current(win, ctx);
}

void li_ctx_swap_buffers(li_win_t win)
{
	li_win_load();
	_li_ctx_swap_buffers(win);
}

void *li_ctx_get_proc_addr(const char *name)
{
	li_win_load();
	return _li_ctx_get_proc_addr(name);
}