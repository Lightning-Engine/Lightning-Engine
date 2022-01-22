#include "li/win.h"

#include <stdio.h>
#include <stdlib.h>

static int running = 1;

void win_cb(li_event_t *event) {
	switch (event->any.type) {
		case li_event_close:
			running = 0;
			break;
		case li_event_key_press:
			printf("key_press { key=%d, state=%d }\n", event->key.key, event->key.state);
			break;
		case li_event_key_release:
			printf("key_release { key=%d, state=%d }\n", event->key.key, event->key.state);
			break;
		case li_event_key_repeat:
			printf("key_repeat { key=%d, state=%d }\n", event->key.key, event->key.state);
			break;
		case li_event_button_press:
			printf("button_press { x=%d, y=%d, button=%d, state=%d }\n", event->button.x, event->button.y, event->button.button, event->button.state);
			break;
		case li_event_button_release:
			printf("button_release { x=%d, y=%d, button=%d, state=%d }\n", event->button.x, event->button.y, event->button.button, event->button.state);
			break;
		case li_event_motion_notify:
			printf("motion { x=%d, y=%d, state=%d }\n", event->motion.x, event->motion.y, event->motion.state);
			break;
	}
}

int li_main(void) {
	li_win_t window;

	li_win_init(win_cb);
	li_win_create(&window, 640, 480);
	li_win_map(window);
	while (running)
		li_win_poll();
	li_win_exit();
	return EXIT_SUCCESS;
}
