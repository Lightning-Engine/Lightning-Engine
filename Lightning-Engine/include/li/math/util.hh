#ifndef LI_MATH_UTIL_HH
#define LI_MATH_UTIL_HH

#include <cstdint>

#define LI_MATH_TYPES(name) \
	using name##c = name<char>; \
	using name##sc = name<signed char>; \
	using name##uc = name<unsigned char>; \
	using name##s = name<short>; \
	using name##ss = name<signed short>; \
	using name##us = name<unsigned short>; \
	using name##i = name<int>; \
	using name##si = name<signed int>; \
	using name##ui = name<unsigned int>; \
	using name##l = name<long>; \
	using name##sl = name<signed long>; \
	using name##ul = name<unsigned long>; \
	using name##ll = name<long long>; \
	using name##sll = name<signed long long>; \
	using name##ull = name<unsigned long long>; \
	using name##f = name<float>; \
	using name##d = name<double>; \
	using name##ld = name<long double>; \
	using name##i8 = name<int8_t>; \
	using name##i16 = name<int16_t>; \
	using name##i32 = name<int32_t>; \
	using name##i64 = name<int64_t>; \
	using name##u8 = name<uint8_t>; \
	using name##u16 = name<uint16_t>; \
	using name##u32 = name<uint32_t>; \
	using name##u64 = name<uint64_t>;

#endif