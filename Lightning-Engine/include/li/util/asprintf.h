#ifndef LI_UTIL_ASPRINTF_H
#define LI_UTIL_ASPRINTF_H

#include "li/memory.h"
#include <stdarg.h>

int li_asprintf(char ** out, const char *restrict format, ...);

int li_avsprintf(char ** out, const char *restrict format, va_list args);

#endif