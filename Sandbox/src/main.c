#include "li/win.h"
#include "li/gl.h"
#include <stdio.h>
#include <stdlib.h>

static li_win_t window1;
static li_win_t window2;
static int running1 = 1;
static int running2 = 2;
static int width = 640;
static int height = 480;

void win_cb(li_event_t *event) {
	switch (event->any.type) {
		case li_event_close:
			if (event->any.window.p == window1.p)
				running1 = 0;
			if (event->any.window.p == window2.p)
				running2 = 0;
			li_win_destroy(event->any.window);
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
			printf("motion_notify { x=%d, y=%d, state=%d }\n", event->motion.x, event->motion.y, event->motion.state);
			break;
		case li_event_window_resize:
			printf("window_resize { width=%d, height=%d }\n", event->resize.width, event->resize.height);
			width = event->resize.width;
			height = event->resize.height;
			break;
		default:
			printf("Unkown event\n");
			break;
	}
}

int main(void) {
	/*li_win_t window;*/
	/*li_ctx_t context;*/
	li_gl_t gl;

	li_win_init(win_cb);
	window1 = li_win_create(width, height);
	window2 = li_win_create(width, height);
	li_win_map(window1);
	li_win_map(window2);
	/*context = li_ctx_create(window);
	li_ctx_make_current(window, context);
	li_gl_init(&gl);
	printf("%s\n", gl.GetString(GL_VERSION));*/
	while (running1 || running2) {
		/*gl.Viewport(0, 0, width, height);
		gl.ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		gl.Clear(GL_COLOR_BUFFER_BIT);
		gl.Begin(GL_TRIANGLES);
		gl.Color3f(1.0f, 0.0f, 0.0f);
		gl.Vertex2f(-0.5f, -0.5f);
		gl.Vertex2f(0.0f, 0.5f);
		gl.Vertex2f(0.5f, -0.5f);
		gl.End();
		gl.Flush();*/
		/*if (running1)
			li_ctx_swap_buffers(window1);*/
		li_win_poll();
	}
	/*li_ctx_destroy(context);*/
	li_win_exit();
	return EXIT_SUCCESS;
}
