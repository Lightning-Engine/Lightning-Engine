#include "li/gl/shader.hh"

#include "li/li.h"
#include "li/window.hh"
#include "li/util/logger.hh"

extern "C" {
	#include "li/memory.h"
}

namespace li {

	namespace opengl {

		program::program() : pid(0), vertex_shader(0), fragment_shader(0), geometry_shader(0),
			compute_shader(0), tess_control_shader(0), tess_eval_shader(0) {
			
		}

		program::~program() {
			destroy();
		}

		bool program::create() {
			pid = gl->CreateProgram();
			if (LI_BUILD_DEBUG && !pid)
				LI_ERROR("Failed to create shader program");
			return (pid);
		}

		void program::destroy() {
			gl->DeleteProgram(pid);
		}

		void program::enable() const {
			gl->UseProgram(pid);
		}

		bool program::link() {
			GLint status, max_len;
			GLchar *buffer;

			if (LI_BUILD_DEBUG && !pid)
				LI_WARN("Dit not initialize shader program");
			
			gl->LinkProgram(pid);
			gl->GetProgramiv(pid, GL_LINK_STATUS, &status);
			if (status == GL_FALSE) {
				gl->GetProgramiv(pid, GL_INFO_LOG_LENGTH, &max_len);

				buffer = new GLchar[max_len];
				gl->GetProgramInfoLog(pid, max_len * sizeof(GLchar), &max_len, buffer);
				destroy();
				LI_ERROR("Failed to link shader shader. {}", buffer);
				delete[] buffer;
				return false;
			}
			detach_all();
			return true;
		}

		void program::_detach_shader(GLuint shader) {
			if (LI_BUILD_DEBUG && !pid)
				LI_WARN("Dit not initialize shader program");
			gl->DetachShader(pid, shader);
		}

		void program::detach_all() {
			if (vertex_shader)
				_detach_shader(vertex_shader);
			if (fragment_shader)
				_detach_shader(fragment_shader);
			if (geometry_shader)
				_detach_shader(geometry_shader);
			if (compute_shader)
				_detach_shader(compute_shader);
			if (tess_control_shader)
				_detach_shader(tess_control_shader);
			if (tess_eval_shader)
				_detach_shader(tess_eval_shader);
		}
	}
}
