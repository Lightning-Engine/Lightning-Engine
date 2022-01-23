#include "li/dl.h"
#include <dlfcn.h>

li_dl_t li_dl_open(const char *filename) {
	li_dl_t dl;
	dl.p = dlopen(filename, RTLD_LAZY);
	return dl;
}

void *li_dl_sym(li_dl_t dl, const char *symbol) {
	return dlsym(dl.p, symbol);
}

void li_dl_close(li_dl_t dl) {
	dlclose(dl.p);
}
