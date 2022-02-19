#include "li/util/asprintf.h"

#include <stdio.h>

int li_asprintf(char **out, const char *restrict format, ...) {
	va_list args;
	int len;

	va_start(args, format);
	len = li_avsprintf(out, format, args);
	va_end(args);
	return (len);
}

int li_avsprintf(char **out, const char *restrict format, va_list args) {
	va_list args_cpy;
	int len;

	va_copy(args_cpy, args);
	len = vsnprintf(NULL, 0, format, args) + 1;

	*out = li_safe_malloc(sizeof(char) * len);
	vsnprintf(*out, len, format, args_cpy);
	va_end(args_cpy);
	return (len);
}