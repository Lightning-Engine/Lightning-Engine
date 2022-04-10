#include "li_dl.h"

union li_sym {
    void   *ptr;
    li_fn_t fun;
};

static li_dl_t *(*li_dl_open_impl)(const char *filename);
static int (*li_dl_close_impl)(li_dl_t *handle);
static void *(*li_dl_psym_impl)(li_dl_t *handle, const char *symbol);
static li_fn_t (*li_dl_fsym_impl)(li_dl_t *handle, const char *symbol);

#if LI_DL_DLFCN
# include <dlfcn.h>
# include <stdlib.h>

struct li_dl {
    void *handle;
};

static int      li_dl_init_dlfcn(void);
static li_dl_t *li_dl_open_dlfcn(const char *filename);
static int      li_dl_close_dlfcn(li_dl_t *handle);
static void    *li_dl_psym_dlfcn(li_dl_t *handle, const char *symbol);
static li_fn_t  li_dl_fsym_dlfcn(li_dl_t *handle, const char *symbol);

static int li_dl_init_dlfcn(void) {
    li_dl_open_impl  = li_dl_open_dlfcn;
    li_dl_close_impl = li_dl_close_dlfcn;
    li_dl_psym_impl  = li_dl_psym_dlfcn;
    li_dl_fsym_impl  = li_dl_fsym_dlfcn;
    return 0;
}

static li_dl_t *li_dl_open_dlfcn(const char *filename) {
    li_dl_t *handle;
    if ((handle = malloc(sizeof *handle))) {
        if ((handle->handle = dlopen(filename, RTLD_LAZY))) {
            return handle;
        }
        free(handle);
    }
    return NULL;
}

static int li_dl_close_dlfcn(li_dl_t *handle) {
    int result = dlclose(handle->handle);
    free(handle);
    return result ? -1 : 0;
}

static void *li_dl_psym_dlfcn(li_dl_t *handle, const char *symbol) {
    return dlsym(handle->handle, symbol);
}

static li_fn_t li_dl_fsym_dlfcn(li_dl_t *handle, const char *symbol) {
    union li_sym sym;
    sym.ptr = li_dl_psym_dlfcn(handle, symbol);
    return sym.fun;
}
#endif

#if LI_DL_WIN32
# include <stdlib.h>
# include <windows.h>

struct li_dl {
    HMODULE handle;
};

static int      li_dl_init_win32(void);
static li_dl_t *li_dl_open_win32(const char *filename);
static int      li_dl_close_win32(li_dl_t *handle);
static void    *li_dl_psym_win32(li_dl_t *handle, const char *symbol);
static li_fn_t  li_dl_fsym_win32(li_dl_t *handle, const char *symbol);

static int li_dl_init_win32(void) {
    li_dl_open_impl  = li_dl_open_win32;
    li_dl_close_impl = li_dl_close_win32;
    li_dl_psym_impl  = li_dl_psym_win32;
    li_dl_fsym_impl  = li_dl_fsym_win32;
    return 0;
}

static li_dl_t *li_dl_open_win32(const char *filename) {
    li_dl_t *handle;
    if ((handle = malloc(sizeof *handle))) {
        if ((handle->handle = LoadLibraryA(filename))) {
            return handle;
        }
        free(handle);
    }
    return NULL;
}

static int li_dl_close_win32(li_dl_t *handle) {
    BOOL result = FreeLibrary(handle->handle);
    free(handle);
    return result ? 0 : -1;
}

static void *li_dl_psym_win32(li_dl_t *handle, const char *symbol) {
    union li_sym sym;
    sym.fun = li_dl_fsym_win32(handle, symbol);
    return sym.ptr;
}

static li_fn_t li_dl_fsym_win32(li_dl_t *handle, const char *symbol) {
    return (li_fn_t) GetProcAddress(handle->handle, symbol);
}
#endif

int li_dl_init(void) {
    static int init = -1;
#if LI_DL_DLFCN
    if (init) {
        init = li_dl_dlfcn_init();
    }
#endif
#if LI_DL_WIN32
    if (init) {
        init = li_dl_win32_init();
    }
#endif
    return init;
}

li_dl_t *li_dl_open(const char *filename) {
    return li_dl_imp_open(filename);
}

void *li_dl_psym(li_dl_t *handle, const char *symbol) {
    return li_dl_imp_psym(handle, symbol);
}

li_fn_t li_dl_fsym(li_dl_t *handle, const char *symbol) {
    return li_dl_imp_fsym(handle, symbol);
}

int li_dl_close(li_dl_t *handle) {
    return li_dl_imp_close(handle);
}
