#include "li/dl.h"
#include <libloaderapi.h>

li_dl_t li_dl_open(const char* filename) {
	li_dl_t dl;
	dl.p = LoadLibraryA(filename);
	return dl;
}

void* li_dl_sym(li_dl_t dl, const char* symbol) {
	return GetProcAddress(dl.p, symbol);
}

void li_dl_close(li_dl_t dl) {
	FreeLibrary(dl.p);
}