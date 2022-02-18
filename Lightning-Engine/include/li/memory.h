#ifndef LI_MEMORY_H
#define LI_MEMORY_H

#include <stddef.h>

void *li_safe_malloc(size_t size);
void *li_safe_calloc(size_t num, size_t size);

#endif
