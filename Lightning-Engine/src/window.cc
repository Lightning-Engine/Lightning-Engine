#include "li/window.hh"
#include <stdlib.h>

namespace li {
	window::window(int width, int height) : width(width), height(height) {
		win = li_win_create(width, height);
		li_win_set_data(win, this);
		ctx = li_ctx_create(win, 4);
		li_win_map(win);
		li_ctx_make_current(win, ctx);
		li_gl_init(&gl);
	}

	window::~window() {
		li_ctx_destroy(win, ctx);
		li_win_destroy(win);
	}

	void window::make_current() {
		li_ctx_make_current(win, ctx);
	}

	void window::swap_buffers() {
		li_ctx_swap_buffers(win, ctx);
	}

	const li::gl &window::get_gl() {
		return gl;
	}

	void window::poll() {
		li_win_poll();
	}

	void window::handle_event(li_event_t *event) {
		li::window *window = (li::window *) li_win_get_data(event->any.window);
		switch (event->any.type) {
			case li_event_window_close: {
				li::window_close_event window_close_event;
				window->dispatcher(window_close_event);
				break;
			}
			case li_event_key_press: {
				li::key_press_event key_press_event;
				key_press_event.key = event->key.key;
				key_press_event.state = event->key.state;
				window->dispatcher(key_press_event);
				break;
			}
			case li_event_key_release: {
				li::key_release_event key_release_event;
				key_release_event.key = event->key.key;
				key_release_event.state = event->key.state;
				window->dispatcher(key_release_event);
				break;
			}
			case li_event_key_repeat: {
				li::key_repeat_event key_repeat_event;
				key_repeat_event.key = event->key.key;
				key_repeat_event.state = event->key.state;
				window->dispatcher(key_repeat_event);
				break;
			}
			case li_event_button_press: {
				li::button_press_event button_press_event;
				button_press_event.x = event->button.x;
				button_press_event.y = event->button.y;
				button_press_event.button = event->button.button;
				button_press_event.state = event->button.state;
				window->dispatcher(button_press_event);
				break;
			}
			case li_event_button_release: {
				li::button_release_event button_release_event;
				button_release_event.x = event->button.x;
				button_release_event.y = event->button.y;
				button_release_event.button = event->button.button;
				button_release_event.state = event->button.state;
				window->dispatcher(button_release_event);
				break;
			}
			case li_event_motion_notify: {
				li::motion_notify_event motion_notify_event;
				motion_notify_event.x = event->motion.x;
				motion_notify_event.y = event->motion.y;
				motion_notify_event.state = event->motion.state;
				window->dispatcher(motion_notify_event);
				break;
			}
			case li_event_window_resize: {
				li::window_resize_event window_resize_event;
				window->width = window_resize_event.width = event->resize.width;
				window->height = window_resize_event.height = event->resize.height;
				window->dispatcher(window_resize_event);
				break;
			}
		}
	}
}

static void _init() {
	li_win_init(li::window::handle_event);
	atexit(li_win_exit);
}

static int _init_var = (_init(), 0);