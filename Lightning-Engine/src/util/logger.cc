#include "li/util/logger.hh"

#include <iostream>
#include <iomanip>

extern "C" {
	#include "li/memory.h"
}

namespace li {

	int log_default_fmt(char **out, const char *str, void *param) {
		int len;
		const char *col_start = "";
		const char *col_end = "";
		const char *tcol_start = "";
		const char *tcol_end = "";
		std::stringstream ss;
		std::string string;

#ifndef _WIN32
		tcol_start = "\033[37m";
		tcol_end = "\033[0m";
		col_start = "\033[37m";
		col_end = "\033[0m";
		if (std::strcmp((const char *) param, "trace") == 0) {
			col_start = "\x1b[36m";
		} else if (std::strcmp((const char *) param, "debug") == 0) {
			col_start = "\x1b[34m";
		} else if (std::strcmp((const char *) param, "info") == 0) {
			col_start = "\x1b[32m";
		} else if (std::strcmp((const char *) param, "warning") == 0) {
			col_start = "\x1b[33m";
		} else if (std::strcmp((const char *) param, "error") == 0) {
			col_start = "\x1b[31m";
		} else if (std::strcmp((const char *) param, "fatal") == 0) {
			col_start = "\x1b[35m";
		}
#endif

		std::time_t t = std::time(nullptr);
		ss << tcol_start << "[" << std::put_time(std::localtime(&t), "%F %T") << "]: " << tcol_end;
		ss << col_start << str << col_end << std::endl;
		string = ss.str();
		*out = (char*) li_safe_malloc(string.size() + 1);
		len = string.copy(*out, string.size());
		(*out)[len] = '\0';
		return len;
	}

	void li_stdout_sink(li_log_str_t *str, void *param) {
		(void) param;
		fwrite(str->str, 1, str->len, stdout);
	}

	logger::~logger() {
		li_log_destroy(&int_logger);
	}

	bool logger::init() {
		li_log_init(&int_logger);
		li_log_set_fmt(&int_logger, log_default_fmt, (void*) logger_name.c_str());
		li_log_add_sink(&int_logger, li_stdout_sink, (void*) logger_name.c_str());
		return true;
	}

}