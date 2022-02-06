#ifndef LI_EVENT_HH
#define LI_EVENT_HH

extern "C" {
	#include "li/keymap.h"
}

namespace li {
	class event { };

	class window_event : public event { };

	class window_resize_event : public window_event {
	public:
		int width;
		int height;
	};

	class window_close_event : public window_event { };

	class mouse_event : public window_event {
	public:
		int x;
		int y;
		li_key_state_t state;
	};

	class button_event : public mouse_event {
	public:
		li_button_t button;
	};
	
	class button_press_event : public button_event { };

	class button_release_event : public button_event { };

	class motion_notify_event : public mouse_event { };

	class key_event : public window_event {
	public:
		li_key_state_t state;
		li_key_t key;
	};

	class key_trigger_event : public key_event { };

	class key_press_event : public key_trigger_event { };
	
	class key_release_event : public key_event { };

	class key_repeat_event : public key_trigger_event { };
}

#endif