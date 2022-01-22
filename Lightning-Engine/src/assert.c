#include "li/assert.h"
#include <stdio.h>
#include <stdlib.h>

void li_assert_impl(int condition, const char* file, int line) {
	if (!condition) {
		fprintf(stderr, "Assertion failed: %s:%d\n", file, line);
		abort();
	}
}
