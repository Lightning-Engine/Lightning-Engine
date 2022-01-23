#ifndef LI_WIN_H
#define LI_WIN_H

#include "li/keymap.h"

typedef union li_win {
	void *p;
	unsigned long lu;
} li_win_t;

typedef union li_ctx {
	void *p;
} li_ctx_t;

typedef enum li_event_type {
	li_event_close,
	li_event_key_press,
	li_event_key_release,
	li_event_key_repeat,
	li_event_button_press,
	li_event_button_release,
	li_event_motion_notify,
	li_event_window_resize,
} li_event_type_t;

typedef struct li_event_any {
	li_event_type_t type;
	li_win_t window;
} li_event_any_t;

typedef struct li_event_key {
	li_event_any_t any;
	li_key_t key;
	li_key_state_t state;
} li_event_key_t;

typedef struct li_event_button {
	li_event_any_t any;
	int x;
	int y;
	li_button_t button;
	li_key_state_t state;
} li_event_button_t;

typedef struct li_event_motion {
	li_event_any_t any;
	int x;
	int y;
	li_key_state_t state;
} li_event_motion_t;

typedef struct li_event_resize {
	li_event_any_t any;
	int width;
	int height;
} li_event_resize_t;

typedef union li_event {
	li_event_any_t any;
	li_event_key_t key;
	li_event_button_t button;
	li_event_motion_t motion;
	li_event_resize_t resize;
} li_event_t;

void li_win_init(void (*cb)(li_event_t*));
void li_win_exit(void);
void li_win_poll(void);
li_win_t li_win_create(int width, int height);
void li_win_destroy(li_win_t win);
void li_win_map(li_win_t win);

li_ctx_t li_ctx_create(li_win_t win);
void li_ctx_destroy(li_ctx_t ctx);
void li_ctx_make_current(li_win_t win, li_ctx_t ctx);
void li_ctx_swap_buffers(li_win_t win);
void *li_ctx_get_proc_addr(const char *name);

#endif
