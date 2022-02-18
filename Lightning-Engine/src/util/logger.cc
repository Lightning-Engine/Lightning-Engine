#include "li/util/logger.hh"

#include <iostream>

extern "C" {
	#include "li/memory.h"
}

namespace li {

	void log_cooler_callback(li_sink_t *sink, li_log_str_t *str) {
		printf("%s", str->str);
		(void) sink;
	}

	int log_cool_fmt(char **out, const char *str) {
		int len;

		len = snprintf(NULL, 0, "[cool]: %s\n", str) + 1;
		*out = (char*) li_safe_malloc(len);
		snprintf(*out, len, "[cool]: %s\n", str);
		return len;
	}

	logger::~logger() {
		// li_log_destroy(&int_logger);
	}

	bool logger::init() {
		li_log_init(&int_logger);
		li_log_set_fmt(&int_logger, log_cool_fmt);
		li_log_add_sink(&int_logger, log_cooler_callback, NULL);
		return true;
	}

}