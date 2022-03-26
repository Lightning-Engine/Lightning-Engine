#ifndef LI_GL_SHADER_HH
#define LI_GL_SHADER_HH

#include "li/li.h"
#include "li/window.hh"
#include "li/util/logger.hh"
#include "li/gl.h"
#include <string>

namespace li {
	namespace opengl {
		enum shader_type {
			vertex_shader = GL_VERTEX_SHADER,
			fragment_shader = GL_FRAGMENT_SHADER,
			geometry_shader = GL_GEOMETRY_SHADER,
			compute_shader = GL_COMPUTE_SHADER,
			tess_control_shader = GL_TESS_CONTROL_SHADER,
			tess_eval_shader = GL_TESS_EVALUATION_SHADER
		};

		class shader_compile_error : public std::runtime_error {
		public:
			shader_compile_error(const std::string &msg) : std::runtime_error(msg) { }
		};

		class program_link_error : public std::runtime_error {
		public:
			program_link_error(const std::string &msg) : std::runtime_error(msg) { }
		};

		template<shader_type Type>
		class shader {
			GLuint id = 0;
		public:
			shader() = default;
			shader(const shader&) = delete;
			shader &operator=(const shader&) = delete;

			~shader() {
				gl->DeleteShader(id);
			}

			void create() {
				LI_WARN_IF(id != 0, "shader already created");
				id = gl->CreateShader(Type);
				// TODO: check error?
			}

			operator GLuint() {
				LI_WARN_IF(id == 0, "shader not created");
				return id;
			}

			void source(const char *string) {
				gl->ShaderSource(*this, 1, &string, 0);
			}
			
			void compile() {
				GLint status, max_len;
				std::vector<GLchar> buffer;

				gl->CompileShader(*this);
				gl->GetShaderiv(*this, GL_COMPILE_STATUS, &status);
				if (status == GL_FALSE) {
					gl->GetShaderiv(*this, GL_INFO_LOG_LENGTH, &max_len);
					buffer.resize(max_len);
					gl->GetShaderInfoLog(id, max_len, nullptr, buffer.data());
					throw shader_compile_error(buffer.data());
				}
			}
		};
	
		class program {
			GLuint id = 0;
		public:
			program() = default;
			program(const program&) = delete;
			program &operator=(const program&) = delete;

			~program() {
				gl->DeleteProgram(id);
			}

			void create() {
				LI_WARN_IF(id != 0, "program already created");
				id = gl->CreateProgram();
				// TODO: check error?
			}

			operator GLuint() {
				LI_WARN_IF(id == 0, "program not created");
				return id;
			}

			void use() {
				gl->UseProgram(*this);
			}

			template<shader_type Type>
			void attach(shader<Type> &shader) {
				gl->AttachShader(*this, shader);
			}

			template<shader_type Type>
			void detach(shader<Type> &shader) {
				gl->DetachShader(*this, shader);
			}

			void link() {
				GLint status, max_len;
				std::vector<GLchar> buffer;

				gl->LinkProgram(*this);
				gl->GetProgramiv(*this, GL_LINK_STATUS, &status);
				if (status == GL_FALSE) {
					gl->GetProgramiv(*this, GL_INFO_LOG_LENGTH, &max_len);
					buffer.resize(max_len);
					gl->GetProgramInfoLog(id, max_len, nullptr, buffer.data());
					throw shader_compile_error(buffer.data());
				}
			}

			template<unsigned int Index>
			void attrib_location(const char *name) {
				gl->BindAttribLocation(*this, Index, name);
			}
		};
	}
}

#endif