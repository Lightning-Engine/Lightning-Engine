#ifndef LI_WINDOW_HH
#define LI_WINDOW_HH

extern "C" {
	#include "li/win.h"
	#include "li/gl.h"
}

namespace li {
	class window {
		li_win_t win;
		li_ctx_t ctx;
		li_gl_t gl;
	public:
		
	};
}

#endif