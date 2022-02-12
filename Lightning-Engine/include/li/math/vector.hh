#ifndef LI_MATH_VECTOR_HH
#define LI_MATH_VECTOR_HH

#include <array>
#include <type_traits>
#include <initializer_list>
#include <iostream>
#include <cstdint>

namespace li {
	template<class T, std::size_t N>
	class vector {
	public:
		std::array<T, N> data;

		vector() = default;

		vector(T value) {
			std::fill(data.begin(), data.end(), value);
		}

		vector(std::initializer_list<T> list) {
			std::copy(list.begin(), list.begin() + std::min(N, list.size()), data.begin());
		}

		template<class... Args>
		vector(Args... args) {
			static_assert(sizeof...(Args) == N, "");
			*this = vector({ T(args)... });
		}

		template<class U, std::size_t M>
		vector(const vector<U, M> &other) {
			static_assert(N <= M, "");
			std::copy(other.data.begin(), other.data.begin() + N, data.begin());
		}

		template<class U, std::size_t M>
		vector(const vector<U, M> &other, T value) {
			std::copy(other.data.begin(), other.data.begin() + std::min(N, M), data.begin());
			std::fill(data.begin() + std::min(N, M), data.end(), value);
		}

		template<class U, std::size_t M>
		vector(const vector<U, M> &other, std::initializer_list<T> list) {
			static_assert(M <= N, "");
			std::copy(other.data.begin(), other.data.end(), data.begin());
			std::copy(list.begin(), list.begin() + std::min(N - M, list.size()), data.begin() + std::min(N, M));
		}

		template<class U, std::size_t M, class... Args>
		vector(const vector<U, M> &other, Args... args) {
			static_assert(sizeof...(Args) + M == N, "");
			*this = vector(other, { T(args)... });
		}

		template<class U, std::size_t M>
		vector &operator=(const vector<U, N> &other) {
			static_assert(N <= M, "");
			std::copy(other.data.begin(), other.data.begin() + N, data.begin());
			return *this;
		}

		const T &operator[](std::size_t i) const {
			return data[i];
		}

		T &operator[](std::size_t i) {
			return data[i];
		}

		const T &x() const { static_assert(0 < N, ""); return data[0]; }
		const T &y() const { static_assert(1 < N, ""); return data[1]; }
		const T &z() const { static_assert(2 < N, ""); return data[2]; }
		const T &w() const { static_assert(3 < N, ""); return data[3]; }
		T &x() { static_assert(0 < N, ""); return data[0]; }
		T &y() { static_assert(1 < N, ""); return data[1]; }
		T &z() { static_assert(2 < N, ""); return data[2]; }
		T &w() { static_assert(3 < N, ""); return data[3]; }

		const T &red() const { static_assert(0 < N, ""); return data[0]; }
		const T &green() const { static_assert(1 < N, ""); return data[1]; }
		const T &blue() const { static_assert(2 < N, ""); return data[2]; }
		const T &alpha() const { static_assert(3 < N, ""); return data[3]; }
		T &red() { static_assert(0 < N, ""); return data[0]; }
		T &green() { static_assert(1 < N, ""); return data[1]; }
		T &blue() { static_assert(2 < N, ""); return data[2]; }
		T &alpha() { static_assert(3 < N, ""); return data[3]; }

		const T &width() const { static_assert(0 < N, ""); return data[0]; }
		const T &height() const { static_assert(1 < N, ""); return data[1]; }
		const T &depth() const { static_assert(2 < N, ""); return data[2]; }
		const T &time() const { static_assert(3 < N, ""); return data[3]; }
		T &width() { static_assert(0 < N, ""); return data[0]; }
		T &height() { static_assert(1 < N, ""); return data[1]; }
		T &depth() { static_assert(2 < N, ""); return data[2]; }
		T &time() { static_assert(3 < N, ""); return data[3]; }
	};

	template<class T, std::size_t N>
	std::ostream &operator<<(std::ostream &os, const vector<T, N> &v) {
		os << "[";
		for (std::size_t i = 0; i < N; ++i) {
			os << v[i];
			if (i != N - 1)
				os << ", ";
		}
		os << "]";
		return os;
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