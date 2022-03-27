#include "li/dl.h"

#include <stddef.h>

#if defined LI_WIN32

int li_dl_open(li_dl_t *dl, const char *path) {
    *dl = LoadLibraryA(path);
    return *dl == NULL;
}

int li_dl_close(li_dl_t *dl) {
    return !FreeLibrary(*dl);
}

void *li_dl_sym(li_dl_t *dl, const char *name) {
    FARPROC proc = GetProcAddress(*dl, name);
    return *(void **) &proc;
}

#else

int li_dl_open(li_dl_t *dl, const char *path) {
    *dl = dlopen(path, RTLD_LAZY);
    return *dl == NULL;
}

int li_dl_close(li_dl_t *dl) {
    return dlclose(*dl);
}

void *li_dl_sym(li_dl_t *dl, const char *name) {
    return dlsym(*dl, name);
}

#endif
