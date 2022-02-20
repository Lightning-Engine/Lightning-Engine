#ifndef LI_UTIL_LOGGER_HH
#define LI_UTIL_LOGGER_HH

#include <string>
#include <sstream>
#include <cstring>
#include <memory>
#include <algorithm>
#include <map>
#include <mutex>

extern "C" {
	#include "li/assert.h"
	#include "li/util/log.h"
}

#define LI_TRACE(...) li::logger::get_logger("trace").log(__VA_ARGS__)
#define LI_DEBUG(...) li::logger::get_logger("debug").log(__VA_ARGS__)
#define LI_INFO(...) li::logger::get_logger("info").log(__VA_ARGS__)
#define LI_WARN(...) li::logger::get_logger("warn").log(__VA_ARGS__)
#define LI_ERROR(...) li::logger::get_logger("error").log(__VA_ARGS__)
#define LI_FATAL(...) li::logger::get_logger("fatal").log(__VA_ARGS__)

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


	template<typename Char>
	class char_traits {};

	template<>
	class char_traits<char> {
	public:
		static constexpr const char *fmt() {
			return "%s";
		}

		static constexpr char bspc() {
			return '{';
		}

		static constexpr char espc() {
			return '}';
		}
	};

	template<>
	class char_traits<wchar_t> {
	public:
		static constexpr const char *fmt() {
			return "%ls";
		}

		static constexpr wchar_t bspc() {
			return L'{';
		}

		static constexpr wchar_t espc() {
			return L'}';
		}
	};


	template<typename Char, typename First>
	void _format(typename basic_string_view<Char>::iterator current, typename basic_string_view<Char>::iterator end,
				std::basic_ostream<Char>& out, First&& first) {
		const Char *spec;

		spec = std::find(current, end, char_traits<Char>::bspc());
		if (!spec || spec == end)
			out << std::basic_string<Char>(current, end - current);
		else {
			const Char *next = spec + 1;

			if (current != spec)
				out << std::basic_string<Char>(current, spec - current);
			li_assert(*next == char_traits<Char>::espc());
			out << first;
			if (next != end)
				out << std::basic_string<Char>(next + 1, end - next);
		}
	}

	template<typename Char, typename First, typename... T>
	void _format(typename basic_string_view<Char>::iterator current, typename basic_string_view<Char>::iterator end,
				std::basic_ostream<Char>& out, First&& first, T&&... args) {
		typename basic_string_view<Char>::iterator spec;

		spec = std::find(current, end, char_traits<Char>::bspc());
		if (!spec || spec == end)
			out << std::basic_string<Char>(current, end - current);
		else {
			const Char *next = spec + 1;

			if (current != spec)
				out << std::basic_string<Char>(current, spec - current);
			li_assert(*next == char_traits<Char>::espc());
			out << first;
			if (next != end)
				_format(next + 1, end, out, std::forward<T>(args)...);
		}
	}

	template<typename Char, typename First>
	void _format(typename basic_string_view<Char>::iterator current, typename basic_string_view<Char>::iterator end,
				std::basic_ostream<Char>& out) {
		out << std::basic_string<Char>(current, end - current);
	}

	template<typename Char, typename... T>
	void format(std::basic_ostream<Char> &out,
											basic_string_view<Char> format, T&&... args) {
		_format(format.begin(), format.end(), out, std::forward<T>(args)...);
	}

	class logger {
		std::string logger_name;
		li_logger_t int_logger;

	public:
		logger(std::string name) : logger_name(name) { }

		~logger();

		bool init();
	public:
		template<typename Char, typename... T>
		void log(basic_string_view<Char> format, T&&... args) {
			std::basic_ostringstream<Char> out;
			li::format(out, format, args...);
			li_log(&int_logger, char_traits<Char>::fmt(), out.str().c_str());
			li_log_flush(&int_logger);
		}

		template<typename Char, typename... T>
		void log(const Char *format, T&&... args) {
			log(basic_string_view<Char>(format), std::forward<T>(args)...);
		}

		static logger &get_logger(const char *name) {
			static std::map<std::string, std::unique_ptr<logger>> loggers;
			static std::mutex loggers_mutex;

			std::lock_guard<std::mutex> lock(loggers_mutex);
			auto it = loggers.find(name);
			if (it == loggers.end()) {
				loggers[name] = std::unique_ptr<logger>(new logger(name));
				it = loggers.find(name);
				it->second->init();
			}
			return *it->second;
		}
	};
}

#endif