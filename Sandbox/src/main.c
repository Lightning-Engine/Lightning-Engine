#include "li/win.h"

#include <stdio.h>
#include <stdlib.h>

static int running = 1;

void li_win_cb(li_win_event_t event) {
	if (event.type == li_win_event_close) {
		running = 0;
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