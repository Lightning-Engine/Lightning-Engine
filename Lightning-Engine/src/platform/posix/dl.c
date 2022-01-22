#include "li/dl.h"
#include "li/entry.h"
#include <dlfcn.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>

li_dl_t li_dl_open(const char *filename) {
	li_dl_t dl;
	dl.p = dlopen(filename, RTLD_LAZY);
	return dl;
}

li_dl_t li_dl_open_rel(const char *filename) {
	char *exe = malloc(strlen(li_argv[0]) + strlen(filename) + 1);
	strcpy(exe, li_argv[0]);
	char *file = dirname(exe);
	strcat(file, "/");
	strcat(file, filename);
	li_dl_t dl = li_dl_open(file);
	free(exe);
	return dl;
}

void *li_dl_sym(li_dl_t dl, const char *symbol) {
	return dlsym(dl.p, symbol);
}

void li_dl_close(li_dl_t dl) {
	dlclose(dl.p);
}
