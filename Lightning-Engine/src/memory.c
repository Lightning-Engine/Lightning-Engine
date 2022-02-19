#include "li/memory.h"
#include "li/assert.h"

#include <stdlib.h>

void *li_safe_malloc(size_t size) {
	void *ptr;

	ptr = malloc(size);
	li_assert(ptr != NULL);
	return ptr;
}

void *li_safe_calloc(size_t num, size_t size) {
	void *ptr;

	ptr = calloc(num, size);
	li_assert(ptr != NULL);
	return ptr;
}