#ifndef LI_GL_BUFFER_HH
#define LI_GL_BUFFER_HH

#include "li/window.hh"
#include "li/gl.h"

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

		template<class T>
		class buffer {
			GLuint id;
		public:
			buffer(const buffer&) = delete;
			buffer &operator=(const buffer&) = delete;
			
			buffer() {
				id = 0;
			}

			buffer(buffer &&other) {
				id = other.id;
				other.id = 0;
			}

			~buffer() {
				gl->DeleteBuffers(1, &id);
			}

			void create() {
				gl->GenBuffers(1, &id);
			}

			operator GLuint() {
				return id;
			}
		};

		template<class T, buffer_binding_target Target>
		class buffer_binding {
		public:
			buffer_binding(const buffer_binding&) = delete;
			buffer_binding(buffer_binding&&) = delete;
			buffer_binding &operator=(const buffer_binding&) = delete;
			buffer_binding &operator=(buffer_binding&&) = delete;

			buffer_binding(buffer<T> &buffer) {
				gl->BindBuffer(Target, buffer);
			}

			~buffer_binding() {
				gl->BindBuffer(Target, 0);
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
			GLuint id;
		public:
			vertex_array(const vertex_array&) = delete;
			vertex_array &operator=(const vertex_array&) = delete;

			vertex_array() : id(0) { }

			vertex_array(vertex_array &&other) {
				id = other.id;
				other.id = 0;
			}

			~vertex_array() {
				gl->DeleteVertexArrays(1, &id);
			}

			void create() {
				gl->GenVertexArrays(1, &id);
			}

			operator GLuint() {
				return id;
			}
		};

		class vertex_array_binding {
		public:
			vertex_array_binding(const vertex_array_binding&) = delete;
			vertex_array_binding(vertex_array_binding&&) = delete;
			vertex_array_binding &operator=(const vertex_array_binding&) = delete;
			vertex_array_binding &operator=(vertex_array_binding&&) = delete;

			vertex_array_binding(vertex_array &vertex_array) {
				gl->BindVertexArray(vertex_array);
			}

			~vertex_array_binding() {
				gl->BindVertexArray(0);
			}

			template<unsigned int Index>
			void enable() {
				gl->EnableVertexAttribArray(Index);
			}

			template<unsigned int Index>
			void disable() {
				gl->DisableVertexAttribArray(Index);
			}

			template<unsigned int Index, class T, bool Normalized = false>
			void attrib(std::size_t size, GLenum type, const void *pointer, const buffer_binding<T, array_buffer> &binding) {
				(void) binding;
				gl->VertexAttribPointer(Index, size, type, Normalized, sizeof(T), pointer);
			}
		};
	}
}

#endif