#ifndef LI_APPLICATION_HH
#define LI_APPLICATION_HH

#include "window.hh"
#include <memory>

namespace li {

	class application {
		bool running;

	public:
		application();
		~application();
		virtual void start();
		virtual void stop();

	protected:
		virtual bool init();
		virtual bool deinit();

		virtual void update();
	};

	class windowed_application : public application {
		std::unique_ptr<window> win;
		int initial_width, initial_height;

		windowed_application();
	public:
		windowed_application(int width, int height);

		virtual bool init() override;
		virtual bool deinit() override;

		virtual void update() override;
	};

}

#endif