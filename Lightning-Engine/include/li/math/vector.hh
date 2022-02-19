#ifndef LI_MATH_VECTOR_HH
#define LI_MATH_VECTOR_HH

#include <array>
#include <type_traits>
#include <initializer_list>
#include <iostream>
#include <cmath>
#include <iomanip>

#include "util.hh"

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
		vector(const vector<U, O> &other) : vector() {
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
		// LI_VECTOR_ASSIGNMENT_OPERATOR(<<=)
		// LI_VECTOR_ASSIGNMENT_OPERATOR(>>=)

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
		os << std::fixed << std::setprecision(3);
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

#define LI_VECTOR_SIZE(size) \
	template<class T> \
	using vec##size = vector<T, size>; \
	LI_MATH_TYPES(vec##size)

	LI_VECTOR_SIZE(2)
	LI_VECTOR_SIZE(3)
	LI_VECTOR_SIZE(4)
}

#endif