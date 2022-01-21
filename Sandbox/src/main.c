#include "li/win.h"

#include <stdio.h>
#include <stdlib.h>

static int running = 1;

void li_win_cb(li_event_t event) {
	switch (event.type) {
		case li_event_close:
			running = 0;
			break;
		case li_event_button_press:
			printf("button press: %d %d %d\n", event.data.button.x, event.data.button.y, event.data.button.button);
			break;
		case li_event_button_release:
			printf("button release: %d %d %d\n", event.data.button.x, event.data.button.y, event.data.button.button);
			break;
		case li_event_motion_notify:
			printf("motion: %d %d\n", event.data.motion.x, event.data.motion.y);
			break;
		case li_event_key_press:
			printf("key press: %d\n", event.data.key.key);
			break;
		case li_event_key_release:
			printf("key release: %d\n", event.data.key.key);
			break;
	}
}

int li_main(void) {
	li_win_t window;

	li_win_init();
	li_win_create(&window, 640, 480);
	li_win_map(window);
	while (running)
		li_win_poll();
	return EXIT_SUCCESS;
}