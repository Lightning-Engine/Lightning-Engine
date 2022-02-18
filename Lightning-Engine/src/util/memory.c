#include "li/memory.h"
#include "li/assert.h"

void *li_safe_malloc(size_t size) {
	void *ptr;

	ptr = malloc(size);
	li_assert(ptr);
	return (ptr);
}
