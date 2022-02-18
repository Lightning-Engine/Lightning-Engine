#ifndef LI_MATH_MATRIX_HH
#define LI_MATH_MATRIX_HH

#include <array>
#include <type_traits>
#include <initializer_list>
#include <iostream>
#include <cstdint>
#include <cmath>

#include "vector.hh"

namespace li {
	template<class T, std::size_t Width, std::size_t Height>
	class matrix {
		std::array<vector<T, Width>, Height> data;
	public:
		matrix() {
			for (std::size_t i = 0; i < Height; i++)
				data[i] = vector<T, Width>();
		}

		matrix(std::initializer_list<vector<T, Width>> list) : matrix() {
			for (std::size_t i = 0; i < Height; i++)
				data[i] = list.begin()[i];
		}

		template<class U, std::size_t Width2, std::size_t Height2>
		matrix(matrix<U, Width2, Height2> other) : matrix() {
			for (std::size_t y = 0; y < std::min(Height, Height2); y++)
				for (std::size_t x = 0; x < std::min(Width, Width2); x++)
					data[y][x] = other.data[y][x];
		}

		vector<T, Width> operator[](std::size_t i) const {
			return data[i];
		}

		vector<T, Width> &operator[](std::size_t i) {
			return data[i];
		}
	};

	template<class T, class U, std::size_t Width, std::size_t Height>
	bool operator==(matrix<T, Width, Height> lhs, matrix<U, Width, Height> rhs) {
		for (std::size_t i = 0; i < Width; i++)
			if (lhs[i] != rhs[i])
				return false;
		return true;
	}

	template<class T, class U, std::size_t Width, std::size_t Height>
	bool operator!=(matrix<T, Width, Height> lhs, matrix<U, Width, Height> rhs) {
		return !(lhs == rhs);
	}

	template<class CharT, class T, std::size_t Width, std::size_t Height>
	std::basic_ostream<CharT> &operator<<(std::basic_ostream<CharT> &os, matrix<T, Width, Height> mat) {
		const char *sep = "[";
		for (std::size_t y = 0; y < Height; y++) {
			os << sep << mat[y];
			sep = "\n ";
		}
		os << "]";
		return os;
	}

	template<class T, std::size_t Width, std::size_t Height>
	matrix<T, Height, Width> transpose(matrix<T, Width, Height> mat) {
		matrix<T, Height, Width> result;
		for (std::size_t y = 0; y < Height; y++)
			for (std::size_t x = 0; x < Width; x++)
				result[y][x] = mat[x][y];
		return result;
	}

	template<class T, std::size_t Size>
	matrix<T, Size, Size> identity() {
		matrix<T, Size, Size> result;
		for (std::size_t i = 0; i < Size; i++)
			result[i][i] = 1;
		return result;
	}

	template<class T, std::size_t Size>
	matrix<T, Size, Size> scale(vector<T, Size> vec) {
		matrix<T, Size, Size> result = identity<T, Size>();
		for (std::size_t i = 0; i < Size; i++)
			result[i][i] *= vec[i];
		return result;
	}

	template<class T, std::size_t Size>
	matrix<T, Size, Size> translate(vector<T, Size> vec) {
		matrix<T, Size, Size> result = identity<T, Size>();
		for (std::size_t i = 0; i < Size; i++)
			result[i][Size - 1] += vec[i];
		return result;
	}

	template<class T, class U, std::size_t Size, std::size_t Width, std::size_t Height>
	auto operator*(matrix<T, Size, Height> lhs, matrix<U, Width, Size> rhs) -> matrix<decltype(lhs[0][0] * rhs[0][0]), Width, Height> {
		matrix<decltype(lhs[0][0] * rhs[0][0]), Width, Height> result;
		for (std::size_t y = 0; y < Height; y++)
			for (std::size_t x = 0; x < Width; x++)
				for (std::size_t i = 0; i < Size; i++)
					result[y][x] += lhs[y][i] * rhs[i][x];
		return result;
	}

	template<class T, class U, std::size_t Width, std::size_t Height>
	auto operator*(matrix<T, Width, Height> lhs, vector<U, Width> rhs) -> vector<decltype(lhs[0][0] * rhs[0]), Height> {
		vector<decltype(lhs[0][0] * rhs[0]), Height> result;
		for (std::size_t y = 0; y < Height; y++)
			for (std::size_t x = 0; x < Width; x++)
				result[y] += lhs[y][x] * rhs[x];
		return result;
	}
}

#endif