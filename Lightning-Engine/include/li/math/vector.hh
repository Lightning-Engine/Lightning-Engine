#ifndef LI_MATH_VECTOR_HH
#define LI_MATH_VECTOR_HH

#include <array>
#include <type_traits>
#include <initializer_list>
#include <iostream>
#include <cstdint>
#include <cmath>

#define X operator[](0)
#define Y operator[](1)
#define Z operator[](2)
#define W operator[](3)
#define R operator[](0)
#define G operator[](1)
#define B operator[](2)
#define A operator[](3)
#define WIDTH operator[](0)
#define HEIGHT operator[](1)
#define DEPTH operator[](2)
#define TIME operator[](3)

namespace li {
	template<class T, std::size_t N>
	class vector {
		std::array<T, N> data;
	public:
		vector() {
			for (std::size_t i = 0; i < N; i++)
				data[i] = 0;
		}

		vector(std::initializer_list<T> list) : vector() {
			for (std::size_t i = 0; i < N; i++)
				data[i] = list.begin()[i];
		}

		template<class U, std::size_t O>
		vector(vector<U, O> other) : vector() {
			for (std::size_t i = 0; i < std::min(N, O); i++)
				data[i] = other.data[i];
		}

		T operator[](std::size_t i) const {
			return data[i];
		}

		T &operator[](std::size_t i) {
			return data[i];
		}

#define LI_VECTOR_ASSIGNMENT_OPERATOR(op) \
		template<class U> \
		vector &operator op(vector<U, N> other) { \
			for (std::size_t i = 0; i < N; i++) \
				data[i] op other[i]; \
			return *this; \
		} \
		template<class U> \
		vector &operator op(U other) { \
			for (std::size_t i = 0; i < N; i++) \
				data[i] op other; \
			return *this; \
		}

		LI_VECTOR_ASSIGNMENT_OPERATOR(+=)
		LI_VECTOR_ASSIGNMENT_OPERATOR(-=)
		LI_VECTOR_ASSIGNMENT_OPERATOR(*=)
		LI_VECTOR_ASSIGNMENT_OPERATOR(/=)
		LI_VECTOR_ASSIGNMENT_OPERATOR(%=)
		LI_VECTOR_ASSIGNMENT_OPERATOR(&=)
		LI_VECTOR_ASSIGNMENT_OPERATOR(|=)
		LI_VECTOR_ASSIGNMENT_OPERATOR(^=)
		LI_VECTOR_ASSIGNMENT_OPERATOR(<<=)
		LI_VECTOR_ASSIGNMENT_OPERATOR(>>=)

		vector &normalize() {
			return *this = normalize(*this);
		}
	};

#define LI_VECTOR_UNARY_OPERATOR(op) \
	template<class T, std::size_t N> \
	auto operator op(vector<T, N> vec) -> vector<decltype(op vec[0]), N> { \
		vector<decltype(op vec[0]), N> result; \
		for (std::size_t i = 0; i < N; i++) \
			result[i] = op vec[i]; \
		return result; \
	}

	LI_VECTOR_UNARY_OPERATOR(+)
	LI_VECTOR_UNARY_OPERATOR(-)
	LI_VECTOR_UNARY_OPERATOR(~)

#define LI_VECTOR_BINARY_OPERATOR(op) \
	template<class T, class U, std::size_t N> \
	auto operator op(vector<T, N> lhs, vector<U, N> rhs) -> vector<decltype(lhs[0] op rhs[0]), N> { \
		vector<decltype(lhs[0] op rhs[0]), N> result; \
		for (std::size_t i = 0; i < N; i++) \
			result[i] = lhs[i] op rhs[i]; \
		return result; \
	} \
	template<class T, class U, std::size_t N> \
	auto operator op(vector<T, N> lhs, U rhs) -> vector<decltype(lhs[0] op rhs), N> { \
		vector<decltype(lhs[0] op rhs), N> result; \
		for (std::size_t i = 0; i < N; i++) \
			result[i] = lhs[i] op rhs; \
		return result; \
	} \
	template<class T, class U, std::size_t N> \
	auto operator op(T lhs, vector<U, N> rhs) -> vector<decltype(lhs op rhs[0]), N> { \
		vector<decltype(lhs op rhs[0]), N> result; \
		for (std::size_t i = 0; i < N; i++) \
			result[i] = lhs op rhs[i]; \
		return result; \
	}

	LI_VECTOR_BINARY_OPERATOR(+)
	LI_VECTOR_BINARY_OPERATOR(-)
	LI_VECTOR_BINARY_OPERATOR(*)
	LI_VECTOR_BINARY_OPERATOR(/)
	LI_VECTOR_BINARY_OPERATOR(%)
	LI_VECTOR_BINARY_OPERATOR(&)
	LI_VECTOR_BINARY_OPERATOR(|)
	LI_VECTOR_BINARY_OPERATOR(^)
	// LI_VECTOR_BINARY_OPERATOR(<<)
	// LI_VECTOR_BINARY_OPERATOR(>>)

	template<class T, class U, std::size_t N>
	bool operator==(vector<T, N> lhs, vector<U, N> rhs) {
		for (std::size_t i = 0; i < N; i++)
			if (lhs[i] != rhs[i])
				return false;
		return true;
	}

	template<class T, class U, std::size_t N>
	bool operator!=(vector<T, N> lhs, vector<U, N> rhs) {
		return !(lhs == rhs);
	}

	template<class CharT, class T, std::size_t N>
	std::basic_ostream<CharT> &operator<<(std::basic_ostream<CharT> &os, vector<T, N> vec) {
		const char *sep = "[";
		for (std::size_t i = 0; i < N; ++i) {
			os << sep << vec[i];
			sep = " ";
		}
		os << "]";
		return os;
	}

	template<class T, std::size_t N>
	double magnitude(vector<T, N> vec) {
		return std::sqrt(dot(vec, vec));
	}

	template<class T, std::size_t N>
	auto normalize(vector<T, N> vec) -> decltype(vec / magnitude(vec)) {
		return vec / magnitude(vec);
	}

	template<class T, class U, std::size_t N>
	auto dot(vector<T, N> lhs, vector<U, N> rhs) -> decltype(lhs[0] * rhs[0]) {
		decltype(lhs[0] * rhs[0]) result = 0;
		for (std::size_t i = 0; i < N; i++)
			result += lhs[i] * rhs[i];
		return result;
	}

	template<class T>
	using vec2 = vector<T, 2>;
	template<class T>
	using vec3 = vector<T, 3>;
	template<class T>
	using vec4 = vector<T, 4>;

	using vec2c = vec2<char>;
	using vec2sc = vec2<signed char>;
	using vec2uc = vec2<unsigned char>;
	using vec2s = vec2<short>;
	using vec2ss = vec2<signed short>;
	using vec2us = vec2<unsigned short>;
	using vec2i = vec2<int>;
	using vec2si = vec2<signed int>;
	using vec2ui = vec2<unsigned int>;
	using vec2l = vec2<long>;
	using vec2sl = vec2<signed long>;
	using vec2ul = vec2<unsigned long>;
	using vec2ll = vec2<long long>;
	using vec2sll = vec2<signed long long>;
	using vec2ull = vec2<unsigned long long>;
	using vec2f = vec2<float>;
	using vec2d = vec2<double>;
	using vec2ld = vec2<long double>;
	using vec2i8 = vec2<int8_t>;
	using vec2i16 = vec2<int16_t>;
	using vec2i32 = vec2<int32_t>;
	using vec2i64 = vec2<int64_t>;
	using vec2u8 = vec2<uint8_t>;
	using vec2u16 = vec2<uint16_t>;
	using vec2u32 = vec2<uint32_t>;
	using vec2u64 = vec2<uint64_t>;

	using vec3c = vec3<char>;
	using vec3sc = vec3<signed char>;
	using vec3uc = vec3<unsigned char>;
	using vec3s = vec3<short>;
	using vec3ss = vec3<signed short>;
	using vec3us = vec3<unsigned short>;
	using vec3i = vec3<int>;
	using vec3si = vec3<signed int>;
	using vec3ui = vec3<unsigned int>;
	using vec3l = vec3<long>;
	using vec3sl = vec3<signed long>;
	using vec3ul = vec3<unsigned long>;
	using vec3ll = vec3<long long>;
	using vec3sll = vec3<signed long long>;
	using vec3ull = vec3<unsigned long long>;
	using vec3f = vec3<float>;
	using vec3d = vec3<double>;
	using vec3ld = vec3<long double>;
	using vec3i8 = vec3<int8_t>;
	using vec3i16 = vec3<int16_t>;
	using vec3i32 = vec3<int32_t>;
	using vec3i64 = vec3<int64_t>;
	using vec3u8 = vec3<uint8_t>;
	using vec3u16 = vec3<uint16_t>;
	using vec3u32 = vec3<uint32_t>;
	using vec3u64 = vec3<uint64_t>;

	using vec4c = vec4<char>;
	using vec4sc = vec4<signed char>;
	using vec4uc = vec4<unsigned char>;
	using vec4s = vec4<short>;
	using vec4ss = vec4<signed short>;
	using vec4us = vec4<unsigned short>;
	using vec4i = vec4<int>;
	using vec4si = vec4<signed int>;
	using vec4ui = vec4<unsigned int>;
	using vec4l = vec4<long>;
	using vec4sl = vec4<signed long>;
	using vec4ul = vec4<unsigned long>;
	using vec4ll = vec4<long long>;
	using vec4sll = vec4<signed long long>;
	using vec4ull = vec4<unsigned long long>;
	using vec4f = vec4<float>;
	using vec4d = vec4<double>;
	using vec4ld = vec4<long double>;
	using vec4i8 = vec4<int8_t>;
	using vec4i16 = vec4<int16_t>;
	using vec4i32 = vec4<int32_t>;
	using vec4i64 = vec4<int64_t>;
	using vec4u8 = vec4<uint8_t>;
	using vec4u16 = vec4<uint16_t>;
	using vec4u32 = vec4<uint32_t>;
	using vec4u64 = vec4<uint64_t>;
}

#endif