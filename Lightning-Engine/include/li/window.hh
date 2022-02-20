#ifndef LI_WINDOW_HH
#define LI_WINDOW_HH

#include "event.hh"
#include "event_dispatcher.hh"
#include "math/vector.hh"

extern "C" {
	#include "li/win.h"
	#include "li/gl.h"
}

namespace li {

	// TODO: be able to specify opengl profile, maybe have a seperate method to construct it?

	class window {
		li_win_t win;
		li_ctx_t ctx;
		li_gl_t gl;

		vec2i size;
	public:
		event_dispatcher<
			window_close_event,
			key_press_event,
			key_release_event,
			key_repeat_event,
			button_press_event,
			button_release_event,
			motion_notify_event,
			window_resize_event
		> dispatcher;

		window(vec2i size);
		window(const window&) = delete;
		window(window&&) = delete;
		window &operator=(const window&) = delete;
		window &operator=(window&&) = delete;
		~window();

		void make_current();
		void swap_buffers();

		inline vec2i get_size() { return size; }

		static void poll();
		static void handle_event(li_event_t *event); // TODO: private
	};
}

#endif