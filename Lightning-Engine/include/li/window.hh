#ifndef LI_WINDOW_HH
#define LI_WINDOW_HH

#include "event.hh"
#include "event_dispatcher.hh"

extern "C" {
	#include "li/win.h"
	#include "li/gl.h"
}

namespace li {
	typedef li_gl_t gl;

	// TODO: now that window can dispatch events to any listener it does not need to be polymorphic so we can remove the virtual constructor

	class window {
		li_win_t win;
		li_ctx_t ctx;
		li_gl_t gl;
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

		window(int width, int height);
		window(const window&) = delete;
		window(window&&) = delete;
		window& operator=(const window&) = delete;
		window& operator=(window&&) = delete;
		virtual ~window();

		void make_current();
		void swap_buffers();
		const li::gl &get_gl();

		static void poll();
	};
}

#endif