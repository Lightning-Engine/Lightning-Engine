#ifndef LI_UTIL_LOGGER_HH
#define LI_UTIL_LOGGER_HH

#include <string>
#include <sstream>
#include <cstring>
#include <memory>

extern "C" {
	#include "li/assert.h"
	#include "li/util/log.h"
}

namespace li {

	template<typename Char>
	class basic_string_view {
	public:
		using size_type = std::size_t;
		using iterator = const Char*;

	protected:
		const Char *data;
		size_type size;

	public:
		constexpr basic_string_view() noexcept : data(NULL), size(0) {}

		constexpr basic_string_view(const Char *data) noexcept : data(data),
			size(std::is_same<Char, char>::value ? std::strlen(reinterpret_cast<const char*>(data))
			: std::char_traits<Char>::length(data)) { }

		constexpr basic_string_view(const Char *data, size_type size) noexcept : data(data), size(size) {}

		constexpr basic_string_view(const basic_string_view<Char>& other) noexcept : data(other.data), size(other.size) {}

		template<typename Traits, typename Allocator>
		constexpr basic_string_view(const std::basic_string<Char, Traits, Allocator>& string) noexcept : data(string.data()), size(string.size()) {}
	
		constexpr iterator begin() const noexcept { return data; }

		constexpr iterator end() const noexcept { return data + size; }
	};

	class logger {
		std::string logger_name;
		li_logger_t int_logger;

	public:
		logger(std::string name) : logger_name(name) { }

		~logger();

		bool init();

	protected:
		template<typename Char, typename First>
		void _format(typename basic_string_view<Char>::iterator current, typename basic_string_view<Char>::iterator end,
					std::basic_ostream<Char>& out, First&& first) {
			const Char *spec;

			spec = (const Char*) std::memchr(current, '{', end - current);
			if (!spec || spec == end)
				out << std::basic_string<Char>(current, end - current);
			else {
				const Char *next = spec + 1;

				if (current != spec)
					out << std::basic_string<Char>(current, spec - current);
				li_assert(*next == '}');
				out << first;
				if (next != end)
					out << std::basic_string<Char>(next + 1, end - next);
			}
		}

		template<typename Char, typename First, typename... T>
		void _format(typename basic_string_view<Char>::iterator current, typename basic_string_view<Char>::iterator end,
					std::basic_ostream<Char>& out, First&& first, T&&... args) {
			const Char *spec;

			spec = (const Char*) std::memchr(current, '{', end - current);
			if (!spec || spec == end)
				out << std::basic_string<Char>(current, end - current);
			else {
				const Char *next = spec + 1;

				if (current != spec)
					out << std::basic_string<Char>(current, spec - current);
				li_assert(*next == '}');
				out << first;
				if (next != end)
					_format(next + 1, end, out, args...);
			}
		}

		template<typename Char, typename... T>
		void format_str(std::basic_ostream<Char>& out,
												basic_string_view<Char> format, T&&... args) {
			_format(format.begin(), format.end(), out, args...);
		}
	public:
		template<typename... T>
		void log(basic_string_view<char> format, T&&... args) {
			std::basic_ostringstream<char> out;
			format_str(out, format, args...);
			li_log(&int_logger, "%s", out.str().c_str());
			li_log_flush(&int_logger);
		}
	};
}

#endif