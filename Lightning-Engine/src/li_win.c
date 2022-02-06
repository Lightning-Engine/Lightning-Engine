#include "li/win.h"
#include "li/dl.h"
#include "li/assert.h"
#include <stdlib.h>

#define LI_LOAD_SYMBOL(name) { void* fn = li_dl_sym(lib, #name); li_assert((_##name = *(name##_type*) &fn) != 0); }
#define LI_FORWARD(ret, rtype, name, args_decl, args_call) \
	typedef rtype (*name##_type)args_decl; \
	static name##_type _##name; \
	rtype name args_decl { \
		_li_win_load(); \
		ret _##name args_call; \
	}

static li_dl_t lib = { .p = NULL };
static int win_loaded = 0;
static void _li_win_load();

LI_FORWARD(, void, li_win_init, (void (*cb)(li_event_t*)), (cb))
LI_FORWARD(, void, li_win_exit, (void), ())
LI_FORWARD(, void, li_win_poll, (void), ())
LI_FORWARD(return, li_win_t, li_win_create, (int width, int height), (width, height))
LI_FORWARD(, void, li_win_destroy, (li_win_t win), (win))
LI_FORWARD(, void, li_win_map, (li_win_t win), (win))
LI_FORWARD(return, li_ctx_t, li_ctx_create, (li_win_t win, int version), (win, version))
LI_FORWARD(, void, li_ctx_destroy, (li_win_t win, li_ctx_t ctx), (win, ctx))
LI_FORWARD(, void, li_ctx_make_current, (li_win_t win, li_ctx_t ctx), (win, ctx))
LI_FORWARD(, void, li_ctx_swap_buffers, (li_win_t win, li_ctx_t ctx), (win, ctx))
LI_FORWARD(return, void*, li_ctx_get_proc_addr, (const char* name), (name))
LI_FORWARD(, void, li_win_set_data, (li_win_t win, void *data), (win, data))
LI_FORWARD(return, void*, li_win_get_data, (li_win_t win), (win))

static void _li_win_load(void) {
	if (!win_loaded) {
		if (lib.p == NULL)
			lib = li_dl_open("libliengine_win_xlib.so");
		if (lib.p == NULL)
			lib = li_dl_open("liengine_win_win32.dll");
		if (lib.p == NULL)
			lib = li_dl_open("libliengine_win_cocoa.so");
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
		LI_LOAD_SYMBOL(li_win_set_data)
		LI_LOAD_SYMBOL(li_win_get_data)
		win_loaded = 1;
	}
}
