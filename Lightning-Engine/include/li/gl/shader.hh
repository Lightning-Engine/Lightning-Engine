#ifndef LI_GL_SHADER_HH
#define LI_GL_SHADER_HH

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

		template<shader_type Type>
		class shader {
			GLuint id;

		public:
			shader() : id(0) {

			}

			~shader() {
				if (id)
					destroy();
			}

			bool create() {
				id = gl->CreateShader(Type);
				return id;
			}
			
			bool compile() {
				GLint status, max_len;
				GLchar *buffer;

				if (LI_BUILD_DEBUG && !id)
					LI_WARN("Dit not initialize shader");
				gl->CompileShader(id);
				gl->GetShaderiv(id, GL_COMPILE_STATUS, &status);
				if (status == GL_FALSE) {
					gl->GetShaderiv(id, GL_INFO_LOG_LENGTH, &max_len);
					
					buffer = new GLchar[max_len];
					gl->GetShaderInfoLog(id, max_len * sizeof(GLchar), &max_len, buffer);
					destroy();
					LI_ERROR("Failed to compile shader. {}", buffer);
					delete[] buffer;
					return false;
				}
				return true;
			}

			void set_source(const std::string& source) {
				if (LI_BUILD_DEBUG && !id)
				LI_WARN("Dit not initialize shader");
				const GLchar* gl_source = (const GLchar*) source.c_str();
				gl->ShaderSource(id, 1, &gl_source, 0);
			}

			void destroy() {
				gl->DeleteShader(id);
			}

			inline GLuint get_id() const { return id; }
		};

	
		class program {
			GLuint pid;
			GLuint vertex_shader, fragment_shader, geometry_shader, compute_shader,
				tess_control_shader, tess_eval_shader;

		public:
			program();

			~program();

			template<shader_type Type>
			constexpr GLuint& get_shader();

			bool create();

			void destroy();

			void enable() const;

			template<shader_type Type>
			void attach_shader(const shader<Type>& shader) {
				if (LI_BUILD_DEBUG && !pid)
				LI_WARN("Dit not initialize shader program");
				gl->AttachShader(pid, shader.get_id());
				get_shader<Type>() = shader.get_id();
			}

			template<shader_type Type>
			void detach_shader(const shader<Type>& shader) {
				_detach_shader(shader.get_id());
				get_shader<Type>() = 0;
			}

			bool link();

			inline GLuint get_pid() const { return pid; }

		protected:
			void _detach_shader(GLuint shader);

			void detach_all();
		};

		template<>
		constexpr GLuint& program::get_shader<shader_type::vertex_shader>() { return vertex_shader; }

		template<>
		constexpr GLuint& program::get_shader<shader_type::fragment_shader>() { return fragment_shader; }

		template<>
		constexpr GLuint& program::get_shader<shader_type::geometry_shader>() { return geometry_shader; }

		template<>
		constexpr GLuint& program::get_shader<shader_type::compute_shader>() { return compute_shader; }

		template<>
		constexpr GLuint& program::get_shader<shader_type::tess_control_shader>() { return tess_control_shader; }

		template<>
		constexpr GLuint& program::get_shader<shader_type::tess_eval_shader>() { return tess_eval_shader; }
	}
}

#endif