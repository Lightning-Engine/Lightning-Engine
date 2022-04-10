#include "li_dl.h"

#include <assert.h>
#include <stdlib.h>

union li_dl_symbol {
    li_dl_sym_t sym;
    li_dl_fun_t fun;
};

static int li_dl_init_result = -1;
static li_dl_t (*li_dl_open_impl)(const char *name);
static int (*li_dl_close_impl)(li_dl_t dl);
static li_dl_sym_t (*li_dl_sym_impl)(li_dl_t dl, const char *name);
static li_dl_fun_t (*li_dl_fun_impl)(li_dl_t dl, const char *name);

#if LI_DL_DLFCN
# include <dlfcn.h>

struct li_dl {
    void *handle;
};

static int         li_dl_init_dlfcn(void);
static li_dl_t     li_dl_open_dlfcn(const char *name);
static int         li_dl_close_dlfcn(li_dl_t dl);
static li_dl_sym_t li_dl_sym_dlfcn(li_dl_t dl, const char *name);
static li_dl_fun_t li_dl_fun_dlfcn(li_dl_t dl, const char *name);

static int li_dl_init_dlfcn(void) {
    li_dl_open_impl  = li_dl_open_dlfcn;
    li_dl_close_impl = li_dl_close_dlfcn;
    li_dl_sym_impl   = li_dl_sym_dlfcn;
    li_dl_fun_impl   = li_dl_fun_dlfcn;
    return 0;
}

static li_dl_t li_dl_open_dlfcn(const char *name) {
    li_dl_t dl;
    dl = malloc(sizeof *dl);
    if (dl != NULL) {
        dl->handle = dlopen(name, RTLD_LAZY);
        if (dl->handle != NULL) {
            return dl;
        }
        free(dl);
    }
    return NULL;
}

static int li_dl_close_dlfcn(li_dl_t dl) {
    int result;
    result = dlclose(dl->handle);
    free(dl);
    return result == 0 ? 0 : -1;
}

static li_dl_sym_t li_dl_sym_dlfcn(li_dl_t dl, const char *name) {
    return dlsym(dl->handle, name);
}

static li_dl_fun_t li_dl_fun_dlfcn(li_dl_t dl, const char *name) {
    union li_dl_symbol symbol;
    symbol.sym = li_dl_sym_dlfcn(dl, name);
    return symbol.fun;
}
#endif

#if LI_DL_WIN32
# include <windows.h>

struct li_dl {
    HMODULE handle;
};

static int         li_dl_init_win32(void);
static li_dl_t     li_dl_open_win32(const char *name);
static int         li_dl_close_win32(li_dl_t dl);
static li_dl_sym_t li_dl_sym_win32(li_dl_t dl, const char *name);
static li_dl_fun_t li_dl_fun_win32(li_dl_t dl, const char *name);

static int li_dl_init_win32(void) {
    li_dl_open_impl  = li_dl_open_win32;
    li_dl_close_impl = li_dl_close_win32;
    li_dl_sym_impl   = li_dl_sym_win32;
    li_dl_fun_impl   = li_dl_fun_win32;
    return 0;
}

static li_dl_t li_dl_open_win32(const char *name) {
    li_dl_t dl;
    dl = malloc(sizeof *dl);
    if (dl != NULL) {
        dl->handle = LoadLibraryA(name);
        if (dl->handle != NULL) {
            return dl;
        }
        free(dl);
    }
    return NULL;
}

static int li_dl_close_win32(li_dl_t dl) {
    BOOL result;
    result = FreeLibrary(dl->handle);
    free(dl);
    return result ? 0 : -1;
}

static li_dl_sym_t li_dl_sym_win32(li_dl_t dl, const char *name) {
    union li_dl_symbol symbol;
    symbol.fun = li_dl_fun_win32(dl, name);
    return symbol.sym;
}

static li_dl_fun_t li_dl_fun_win32(li_dl_t dl, const char *name) {
    return (li_dl_fun_t) GetProcAddress(dl->handle, name);
}
#endif

#if LI_DL_DYLD
# include <mach-o/dyld.h>

struct li_dl {
    NSModule handle;
};

static int         li_dl_init_dyld(void);
static li_dl_t     li_dl_open_dyld(const char *name);
static int         li_dl_close_dyld(li_dl_t dl);
static li_dl_sym_t li_dl_sym_dyld(li_dl_t dl, const char *name);
static li_dl_fun_t li_dl_fun_dyld(li_dl_t dl, const char *name);

static int li_dl_init_dyld(void) {
    li_dl_open_impl  = li_dl_open_dyld;
    li_dl_close_impl = li_dl_close_dyld;
    li_dl_sym_impl   = li_dl_sym_dyld;
    li_dl_fun_impl   = li_dl_fun_dyld;
    return 0;
}

static li_dl_t li_dl_open_dyld(const char *name) {
    li_dl_t                     dl;
    NSObjectFileImage           image;
    NSObjectFileImageReturnCode result;
    dl = malloc(sizeof *dl);
    if (dl != NULL) {
        result = NSCreateObjectFileImageFromFile(name, &image);
        if (result == NSObjectFileImageSuccess) {
            dl->handle = NSLinkModule(image, name, NSLINKMODULE_OPTION_NONE);
            NSDestroyObjectFileImage(image);
            if (dl->handle != NULL) {
                return dl;
            }
        }
        free(dl);
    }
    return NULL;
}

static int li_dl_close_dyld(li_dl_t dl) {
    bool result;
    result = NSUnLinkModule(dl->handle, NSUNLINKMODULE_OPTION_NONE);
    free(dl);
    return result ? 0 : -1;
}

static li_dl_sym_t li_dl_sym_dyld(li_dl_t dl, const char *name) {
    NSSymbol symbol;
    symbol = NSLookupSymbolInModule(dl->handle, name);
    return symbol ? NSAddressOfSymbol(symbol) : NULL;
}

static li_dl_fun_t li_dl_fun_dyld(li_dl_t dl, const char *name) {
    union li_dl_symbol symbol;
    symbol.sym = li_dl_sym_dyld(dl, name);
    return symbol.fun;
}
#endif

int li_dl_init(void) {
#if LI_DL_DLFCN
    if (li_dl_init_result != 0) {
        li_dl_init_result = li_dl_init_dlfcn();
    }
#endif
#if LI_DL_WIN32
    if (li_dl_init_result != 0) {
        li_dl_init_result = li_dl_init_win32();
    }
#endif
#if LI_DL_DYLD
    if (li_dl_init_result != 0) {
        li_dl_init_result = li_dl_init_dyld();
    }
#endif
    return li_dl_init_result;
}

li_dl_t li_dl_open(const char *name) {
    assert(li_dl_init_result == 0);
    assert(name != NULL);
    return li_dl_open_impl(name);
}

int li_dl_close(li_dl_t dl) {
    assert(li_dl_init_result == 0);
    assert(dl != NULL);
    return li_dl_close_impl(dl);
}

li_dl_sym_t li_dl_sym(li_dl_t dl, const char *name) {
    assert(li_dl_init_result == 0);
    assert(dl != NULL);
    assert(name != NULL);
    return li_dl_sym_impl(dl, name);
}

li_dl_fun_t li_dl_fun(li_dl_t dl, const char *name) {
    assert(li_dl_init_result == 0);
    assert(dl != NULL);
    assert(name != NULL);
    return li_dl_fun_impl(dl, name);
}
