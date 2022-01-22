#include "li/dl.h"
#include <libloaderapi.h>

li_dl_t li_dl_open(const char* filename) {
	li_dl_t dl;
	dl.p = LoadLibraryA(filename);
	return dl;
}

li_dl_t li_dl_open_rel(const char *filename) {
	return (li_dl_open(filename));
}

void* li_dl_sym(li_dl_t dl, const char* symbol) {
	FARPROC proc = GetProcAddress(dl.p, symbol);
	return *((void**) &proc);
}

void li_dl_close(li_dl_t dl) {
	FreeLibrary(dl.p);
}