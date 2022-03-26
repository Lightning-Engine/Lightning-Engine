#ifndef GL_BUFFER_UTIL_HH
#define GL_BUFFER_UTIL_HH

#include "li/gl.h"

namespace li {
	namespace opengl {
		template<typename T>
		constexpr GLenum _type();

#define LI_OPENGL_TYPE(type, gl_type) \
		template<> \
		constexpr GLenum _type<type>() { \
			return gl_type; \
		}

		LI_OPENGL_TYPE(GLbyte, GL_BYTE)
		LI_OPENGL_TYPE(GLubyte, GL_UNSIGNED_BYTE)
		LI_OPENGL_TYPE(GLshort, GL_SHORT)
		LI_OPENGL_TYPE(GLushort, GL_UNSIGNED_SHORT)
		LI_OPENGL_TYPE(GLint, GL_INT)
		LI_OPENGL_TYPE(GLuint, GL_UNSIGNED_INT)
		LI_OPENGL_TYPE(GLfloat, GL_FLOAT)
		LI_OPENGL_TYPE(GLdouble, GL_DOUBLE)

		template<typename T, typename U>
		class offset {
			void *ptr;
		public:
			template<typename = typename std::enable_if<std::is_same<T, U>::value>::type>
			constexpr offset() : ptr(nullptr) { }
			
			constexpr offset(U T::*member) : ptr(reinterpret_cast<void*>(&(reinterpret_cast<T*>(0)->*member))) { }

			template<typename V>
			constexpr offset(offset<T, V> other, U V::*member) : ptr(reinterpret_cast<void*>(&(reinterpret_cast<V*>(0)->*member))) { }

			constexpr operator void *() const {
				return ptr;
			}
		};
	}
}

#endif