#include "li/window.hh"
#include <iostream>

class sandbox : public li::window {
public:
	bool running = 1;
	int width;
	int height;
	float xp = 0;
	float yp = 0;

	sandbox(int width, int height) : li::window(width, height), width(width), height(height) { }

	void on(li::window_close_event &event) {
		(void) event;
		running = false;
	}

	void on(li::window_resize_event &event) {
		width = event.width;
		height = event.height;
	}

	void on(li::motion_notify_event &event) {
		xp = (float) event.x / width * 2 - 1;
		yp = (float) event.y / height * 2 - 1;
	}
};

int main(void) {
	sandbox sandbox(640, 480);
	sandbox.dispatcher.add_listener<li::window_close_event>(sandbox);
	sandbox.dispatcher.add_listener<li::window_resize_event>(sandbox);
	sandbox.dispatcher.add_listener<li::motion_notify_event>(sandbox);
	sandbox.make_current();
	const li::gl &gl = sandbox.get_gl();

	while (sandbox.running) {
		gl.Viewport(0, 0, sandbox.width, sandbox.height);
		gl.ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		gl.Clear(GL_COLOR_BUFFER_BIT);
		gl.Begin(GL_TRIANGLES);
		gl.Color3f(1.0f, 0.0f, 0.0f);
		gl.Vertex2f(-0.5f + sandbox.xp, -0.5f - sandbox.yp);
		gl.Vertex2f(0.0f + sandbox.xp, 0.5f - sandbox.yp);
		gl.Vertex2f(0.5f + sandbox.xp, -0.5f - sandbox.yp);
		gl.End();
		gl.Flush();
		li::window::poll();
		sandbox.swap_buffers();
	}
}
