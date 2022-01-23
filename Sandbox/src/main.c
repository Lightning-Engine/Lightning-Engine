#include "li/win.h"
#include <GL/gl.h>
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
	li_ctx_t context;

	li_win_init(win_cb);
	window = li_win_create(640, 480);
	li_win_map(window);
	context = li_ctx_create(window);
	li_ctx_make_current(window, context);
	printf("%s\n", glGetString(GL_VERSION));
	
	// glOrtho(0, 640, 480, 0, -1 , 1);
	// glMatrixMode(GL_PROJECTION);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, 1, 20);
	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();

	glBegin(GL_TRIANGLES);
	glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, -0.5f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(0.0f, 0.5f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f); glVertex3f(0.5f, -0.5f, 0.0f);
	glEnd();
	glFlush();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 640, 480);
	while (running) {
			glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
			glColor3f(  1.0f,  0.0f, 0.0f);
			glVertex3f( 0.0f, -1.0f, 0.0f);
			glColor3f(  0.0f,  1.0f, 0.0f);
			glVertex3f(-1.0f,  1.0f, 0.0f);
			glColor3f(  0.0f,  0.0f, 1.0f);
			glVertex3f( 1.0f,  1.0f, 0.0f);
		glEnd();
		DrawAQuad();
		li_win_poll();
		li_ctx_swap_buffers(window);
	}
	li_win_exit();
	return EXIT_SUCCESS;
}
