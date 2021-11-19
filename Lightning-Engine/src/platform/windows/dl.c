#include <libloaderapi.h>
#include "li/dl.h"

li_dl_t li_dl_open(const char* filename) {
	return LoadLibrary(filename);
}

void* li_dl_sym(li_dl_t dl, const char* symbol) {
	return GetProcAddress(dl, symbol);
}

void li_dl_close(li_dl_t dl) {
	FreeLibrary(dl);
}