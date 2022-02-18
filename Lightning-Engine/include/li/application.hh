#ifndef LI_APPLICATION_HH
#define LI_APPLICATION_HH

#include "li/window.hh"
#include <memory>

namespace li {
	class application {
	public:
		bool running;

		application();
		~application();
		void start();
		void stop();

	protected:
		virtual bool init();
		virtual bool deinit();

		virtual void update();
	};

	class windowed_application : public application {
		std::unique_ptr<window> win;
		vec2i initial_size;

		windowed_application();
	public:
		windowed_application(vec2i size);

		virtual bool init() override;
		virtual bool deinit() override;

		virtual void update() override;

		inline window &get_window() const { return *win; }
	};
}

#endif