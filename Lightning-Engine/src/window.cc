#include "li/window.hh"
#include <stdlib.h>

namespace li {
	thread_local li_gl_t *gl;

	window::window(vec2i size) : size(size) {
		win = li_win_create(size.WIDTH, size.HEIGHT);
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
		li::gl = &gl;
		li_ctx_make_current(win, ctx);
	}

	void window::swap_buffers() {
		li_ctx_swap_buffers(win, ctx);
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
				button_press_event.position = { event->button.x, event->button.y };
				button_press_event.button = event->button.button;
				button_press_event.state = event->button.state;
				window->dispatcher(button_press_event);
				break;
			}
			case li_event_button_release: {
				li::button_release_event button_release_event;
				button_release_event.position = { event->button.x, event->button.y };
				button_release_event.button = event->button.button;
				button_release_event.state = event->button.state;
				window->dispatcher(button_release_event);
				break;
			}
			case li_event_motion_notify: {
				li::motion_notify_event motion_notify_event;
				motion_notify_event.position = { event->motion.x, event->motion.y };
				motion_notify_event.state = event->motion.state;
				window->dispatcher(motion_notify_event);
				break;
			}
			case li_event_window_resize: {
				li::window_resize_event window_resize_event;
				window->size = window_resize_event.size = { event->resize.width, event->resize.height };
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