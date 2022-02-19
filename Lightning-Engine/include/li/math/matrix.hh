#ifndef LI_MATH_MATRIX_HH
#define LI_MATH_MATRIX_HH

#include <array>
#include <type_traits>
#include <initializer_list>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>

#include "util.hh"
#include "vector.hh"

namespace li {
	template<class T, std::size_t Height, std::size_t Width>
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

		template<class U, std::size_t Height2, std::size_t Width2>
		matrix(matrix<U, Height2, Width2> other) : matrix() {
			for (std::size_t y = 0; y < std::min(Height, Height2); y++)
				for (std::size_t x = 0; x < std::min(Width, Width2); x++)
					data[y][x] = other[y][x];
		}

		vector<T, Width> operator[](std::size_t i) const {
			return data[i];
		}

		vector<T, Width> &operator[](std::size_t i) {
			return data[i];
		}

		static matrix identity() {
			matrix result;
			for (std::size_t i = 0; i < std::min(Width, Height); i++)
				result[i][i] = 1;
			return result;
		}

		template<class U, std::size_t N>
		static matrix scale(vector<U, N> vec) {
			matrix result = identity();
			for (std::size_t i = 0; i < N; i++)
				result[i][i] *= vec[i];
			return result;
		}

		template<class U, std::size_t N>
		static matrix translate(vector<U, N> vec) {
			matrix result = identity();
			for (std::size_t i = 0; i < N; i++)
				result[i][Width - 1] += vec[i];
			return result;
		}

		template<std::size_t X_, std::size_t Y_>
		static matrix rotate(double angle) {
			matrix result = identity();
			result[X_][X_] = std::cos(angle);
			result[X_][Y_] = -std::sin(angle);
			result[Y_][X_] = std::sin(angle);
			result[Y_][Y_] = std::cos(angle);
			return result;
		}
	};

	template<class T, class U, std::size_t Height, std::size_t Width>
	bool operator==(matrix<T, Height, Width> lhs, matrix<U, Height, Width> rhs) {
		for (std::size_t i = 0; i < Width; i++)
			if (lhs[i] != rhs[i])
				return false;
		return true;
	}

	template<class T, class U, std::size_t Height, std::size_t Width>
	bool operator!=(matrix<T, Height, Width> lhs, matrix<U, Height, Width> rhs) {
		return !(lhs == rhs);
	}

	template<class CharT, class T, std::size_t Height, std::size_t Width>
	std::basic_ostream<CharT> &operator<<(std::basic_ostream<CharT> &os, matrix<T, Height, Width> mat) {
		size_t max_width = 0;
		os << std::fixed << std::setprecision(3);
		for (std::size_t i = 0; i < Height; i++) {
			for (std::size_t j = 0; j < Width; j++) {
				std::stringstream ss;
				ss << std::fixed << std::setprecision(3) << mat[i][j];
				if (ss.str().size() > max_width)
					max_width = ss.str().size();
			}
		}
		const char *sep = "[";
		for (std::size_t i = 0; i < Height; i++) {
			os << sep;
			const char *vsep = "[";
			for (std::size_t j = 0; j < Width; ++j) {
				os << vsep << std::setw(max_width) << mat[i][j];
				vsep = " ";
			}
			os << "]";
			sep = "\n ";
		}
		os << "]";
		return os;
	}

	template<class T, std::size_t Height, std::size_t Width>
	matrix<T, Width, Height> transpose(matrix<T, Height, Width> mat) {
		matrix<T, Width, Height> result;
		for (std::size_t y = 0; y < Height; y++)
			for (std::size_t x = 0; x < Width; x++)
				result[y][x] = mat[x][y];
		return result;
	}

	template<class T, class U, std::size_t Size, std::size_t Height, std::size_t Width>
	auto operator*(matrix<T, Height, Size> lhs, matrix<U, Size, Width> rhs) -> matrix<decltype(lhs[0][0] * rhs[0][0]), Height, Width> {
		matrix<decltype(lhs[0][0] * rhs[0][0]), Height, Width> result;
		for (std::size_t y = 0; y < Height; y++)
			for (std::size_t x = 0; x < Width; x++)
				for (std::size_t i = 0; i < Size; i++)
					result[y][x] += lhs[y][i] * rhs[i][x];
		return result;
	}

	template<class T, class U, std::size_t Height, std::size_t Width>
	auto operator*(matrix<T, Height, Width> lhs, vector<U, Width> rhs) -> vector<decltype(lhs[0][0] * rhs[0]), Height> {
		vector<decltype(lhs[0][0] * rhs[0]), Height> result;
		for (std::size_t y = 0; y < Height; y++)
			for (std::size_t x = 0; x < Width; x++)
				result[y] += lhs[y][x] * rhs[x];
		return result;
	}

#define LI_MATRIX_SQUARE(size) \
	template<class T> \
	using mat##size = matrix<T, size, size>; \
	LI_MATH_TYPES(mat##size)

#define LI_MATRIX_RECTANGLE(width, height) \
	template<class T> \
	using mat##height##width = matrix<T, width, height>; \
	LI_MATH_TYPES(mat##height##width)

	LI_MATRIX_SQUARE(2);
	LI_MATRIX_SQUARE(3);
	LI_MATRIX_SQUARE(4);

	LI_MATRIX_RECTANGLE(2, 2)
	LI_MATRIX_RECTANGLE(2, 3)
	LI_MATRIX_RECTANGLE(2, 4)
	LI_MATRIX_RECTANGLE(3, 2)
	LI_MATRIX_RECTANGLE(3, 3)
	LI_MATRIX_RECTANGLE(3, 4)
	LI_MATRIX_RECTANGLE(4, 2)
	LI_MATRIX_RECTANGLE(4, 3)
	LI_MATRIX_RECTANGLE(4, 4)
}

#endif