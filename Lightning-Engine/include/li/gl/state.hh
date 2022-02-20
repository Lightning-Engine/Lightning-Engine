#ifndef LI_GL_STATE_HH
#define LI_GL_STATE_HH

namespace li {
	namespace opengl {
		extern thread_local li_gl_t *gl;
		extern thread_local gl_state *state;

		class gl_state {
			GLuint vertex_buffer;
			
		};
	}
}