#include "li/application.hh"

extern "C" {
	#include "li/assert.h"
}

namespace li {

	application::application() : running(false) {

	}

	application::~application() {

	}

	void application::start() {
		running = true;
		li_assert(init());
		while (running) {
			update();
		}
	}

	void application::stop() {
		running = false;
		li_assert(deinit());
	}

	void application::update() {

	}

	bool application::init() {
		return true;
	}

	bool application::deinit() {
		return true;
	}

	windowed_application::windowed_application() : windowed_application(500, 500) {

	}

	windowed_application::windowed_application(int width, int height) : initial_width(width), initial_height(height) {

	}

	bool windowed_application::init() {
		win = std::unique_ptr<window>(new window(initial_width, initial_height));
		win->make_current();
		return application::init();
	}

	void windowed_application::update() {
		win->poll();
		win->swap_buffers();
		application::update();
	}

	bool windowed_application::deinit() {
		return application::deinit();
	}
}