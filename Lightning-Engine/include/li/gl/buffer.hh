#ifndef LI_GL_BUFFER_HH
#define LI_GL_BUFFER_HH

#include "li/util/logger.hh"
#include "li/gl/util.hh"
#include "li/window.hh"

#include <cstddef>

namespace li {
	namespace opengl {
		enum buffer_binding_target {
			array_buffer = GL_ARRAY_BUFFER,
			atomic_counter_buffer = GL_ATOMIC_COUNTER_BUFFER,
			copy_read_buffer = GL_COPY_READ_BUFFER,
			copy_write_buffer = GL_COPY_WRITE_BUFFER,
			dispatch_indirect_buffer = GL_DISPATCH_INDIRECT_BUFFER,
			draw_indirect_buffer = GL_DRAW_INDIRECT_BUFFER,
			element_array_buffer = GL_ELEMENT_ARRAY_BUFFER,
			pixel_pack_buffer = GL_PIXEL_PACK_BUFFER,
			pixel_unpack_buffer = GL_PIXEL_UNPACK_BUFFER,
			query_buffer = GL_QUERY_BUFFER,
			shader_storage_buffer = GL_SHADER_STORAGE_BUFFER,
			texture_buffer = GL_TEXTURE_BUFFER,
			transform_feedback_buffer = GL_TRANSFORM_FEEDBACK_BUFFER,
			uniform_buffer = GL_UNIFORM_BUFFER,
		};

		enum buffer_usage {
			static_draw = GL_STATIC_DRAW,
			static_read = GL_STATIC_READ,
			static_copy = GL_STATIC_COPY,
			dynamic_draw = GL_DYNAMIC_DRAW,
			dynamic_read = GL_DYNAMIC_READ,
			dynamic_copy = GL_DYNAMIC_COPY,
			stream_draw = GL_STREAM_DRAW,
			stream_read = GL_STREAM_READ,
			stream_copy = GL_STREAM_COPY,
		};

		enum primitive_kind {
			points = GL_POINTS,
			lines = GL_LINES,
			line_loop = GL_LINE_LOOP,
			line_strip = GL_LINE_STRIP,
			triangles = GL_TRIANGLES,
			triangle_strip = GL_TRIANGLE_STRIP,
			triangle_fan = GL_TRIANGLE_FAN,
		};

		template<typename T>
		class buffer {
			GLuint id = 0;
		public:
			buffer() = default;
			buffer(const buffer&) = delete;
			buffer &operator=(const buffer&) = delete;

			~buffer() {
				gl->DeleteBuffers(1, &id);
			}

			void gen() {
				LI_WARN_IF(id != 0, "buffer already generated");
				gl->GenBuffers(1, &id);
			}

			operator GLuint() {
				LI_WARN_IF(id == 0, "buffer not generated");
				return id;
			}
		};

		template<typename T, buffer_binding_target Target>
		class buffer_binding {
		public:
			void bind(buffer<T> &b) {
				gl->BindBuffer(Target, b);
			}

			template<buffer_usage Usage = static_draw>
			void data(std::size_t size, const T *data) {
				gl->BufferData(Target, sizeof(T) * size, data, Usage);
			}

			void sub_data(std::size_t offset, std::size_t size, const T *data) {
				gl->BufferSubData(Target, sizeof(T) * offset, sizeof(T) * size, data);
			}
		};

		class vertex_array {
			GLuint id = 0;
		public:
			vertex_array() = default;
			vertex_array(const vertex_array&) = delete;
			vertex_array &operator=(const vertex_array&) = delete;

			~vertex_array() {
				gl->DeleteVertexArrays(1, &id);
			}

			void gen() {
				LI_WARN_IF(id != 0, "vertex array already generated");
				gl->GenVertexArrays(1, &id);
			}

			operator GLuint() {
				LI_WARN_IF(id == 0, "vertex array not generated");
				return id;
			}
		};

		class vertex_array_binding {
		public:
			void bind(vertex_array &va) {
				gl->BindVertexArray(va);
			}

			template<unsigned int Index>
			void enable_attrib() {
				gl->EnableVertexAttribArray(Index);
			}

			template<unsigned int Index>
			void disable_attrib() {
				gl->DisableVertexAttribArray(Index);
			}

			template<unsigned int Index, typename T, typename U = T>
			void attrib_pointer(buffer_binding<U, array_buffer> &b, offset<T, U> ofs = offset<T, U>()) {
				(void) b;
				gl->VertexAttribPointer(Index, sizeof(T) / sizeof(typename T::value_type), _type<typename T::value_type>(), GL_FALSE, sizeof(U), ofs);
			}

			template<primitive_kind Mode, typename T>
			void draw_elements(buffer_binding<T, element_array_buffer> &b, std::size_t count, std::size_t ofs = 0) {
				(void) b;
				gl->DrawElements(Mode, count, _type<T>(), (T*) 0 + ofs);
			}
		};
	}
}

#endif