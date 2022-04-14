#include "li_dl.h"

#include <stdlib.h>
#include <string.h>

union li_dl_symbol {
    li_dl_sym_t sym;
    li_dl_fun_t fun;
};

struct li_dl_impl {
    li_dl_t (*open)(const char *name);
    int (*close)(li_dl_t dl);
    li_dl_sym_t (*sym)(li_dl_t dl, const char *name);
    li_dl_fun_t (*fun)(li_dl_t dl, const char *name);
};

struct li_dl_base {
    const struct li_dl_impl *impl;
};

#if LI_DL_DLFCN
# include <dlfcn.h>

struct li_dl_dlfcn {
    struct li_dl_base base;
    void             *handle;
};

static li_dl_t     li_dl_open_dlfcn(const char *name);
static int         li_dl_close_dlfcn(li_dl_t dl);
static li_dl_sym_t li_dl_sym_dlfcn(li_dl_t dl, const char *name);
static li_dl_fun_t li_dl_fun_dlfcn(li_dl_t dl, const char *name);

static const struct li_dl_impl li_dl_impl_dlfcn = {
    li_dl_open_dlfcn, li_dl_close_dlfcn, li_dl_sym_dlfcn, li_dl_fun_dlfcn
};

static li_dl_t li_dl_open_dlfcn(const char *name) {
    struct li_dl_dlfcn *dl_dlfcn;
    dl_dlfcn = malloc(sizeof *dl_dlfcn);
    if (dl_dlfcn != NULL) {
        dl_dlfcn->base.impl = &li_dl_impl_dlfcn;
        dl_dlfcn->handle    = dlopen(name, RTLD_LAZY);
        if (dl_dlfcn->handle != NULL) {
            return dl_dlfcn;
        }
        free(dl_dlfcn);
    }
    return NULL;
}

static int li_dl_close_dlfcn(li_dl_t dl) {
    struct li_dl_dlfcn *dl_dlfcn = dl;
    int                 result;
    result = dlclose(dl_dlfcn->handle);
    free(dl_dlfcn);
    return result == 0 ? 0 : -1;
}

static li_dl_sym_t li_dl_sym_dlfcn(li_dl_t dl, const char *name) {
    struct li_dl_dlfcn *dl_dlfcn = dl;
    return dlsym(dl_dlfcn->handle, name);
}

static li_dl_fun_t li_dl_fun_dlfcn(li_dl_t dl, const char *name) {
    union li_dl_symbol symbol;
    symbol.sym = li_dl_sym_dlfcn(dl, name);
    return symbol.fun;
}
#endif

#if LI_DL_WIN32
# include <libloaderapi.h>

struct li_dl_win32 {
    struct li_dl_base base;
    HMODULE           module;
};

static li_dl_t     li_dl_open_win32(const char *name);
static int         li_dl_close_win32(li_dl_t dl);
static li_dl_sym_t li_dl_sym_win32(li_dl_t dl, const char *name);
static li_dl_fun_t li_dl_fun_win32(li_dl_t dl, const char *name);

static const struct li_dl_impl li_dl_impl_win32 = {
    li_dl_open_win32, li_dl_close_win32, li_dl_sym_win32, li_dl_fun_win32
};

static li_dl_t li_dl_open_win32(const char *name) {
    struct li_dl_win32 *dl_win32;
    dl_win32 = malloc(sizeof *dl_win32);
    if (dl_win32 != NULL) {
        dl_win32->base.impl = &li_dl_impl_win32;
        dl_win32->module    = LoadLibraryA(name);
        if (dl_win32->module != NULL) {
            return dl_win32;
        }
        free(dl_win32);
    }
    return NULL;
}

static int li_dl_close_win32(li_dl_t dl) {
    struct li_dl_win32 *dl_win32 = dl;
    BOOL                result;
    result = FreeLibrary(dl_win32->module);
    free(dl_win32);
    return result ? 0 : -1;
}

static li_dl_sym_t li_dl_sym_win32(li_dl_t dl, const char *name) {
    union li_dl_symbol symbol;
    symbol.fun = li_dl_fun_win32(dl, name);
    return symbol.sym;
}

static li_dl_fun_t li_dl_fun_win32(li_dl_t dl, const char *name) {
    struct li_dl_win32 *dl_win32 = dl;
    return (li_dl_fun_t) GetProcAddress(dl_win32->module, name);
}
#endif

#if LI_DL_DYLD
# include <mach-o/dyld.h>

struct li_dl_dyld {
    struct li_dl_base         base;
    NSModule                  module;
    const struct mach_header *image;
};

static li_dl_t     li_dl_open_dyld(const char *name);
static int         li_dl_close_dyld(li_dl_t dl);
static li_dl_sym_t li_dl_sym_dyld(li_dl_t dl, const char *name);
static li_dl_fun_t li_dl_fun_dyld(li_dl_t dl, const char *name);

static const struct li_dl_impl li_dl_impl_dyld = {
    li_dl_open_dyld, li_dl_close_dyld, li_dl_sym_dyld, li_dl_fun_dyld
};

static li_dl_t li_dl_open_dyld(const char *name) {
    struct li_dl_dyld          *dl_dyld;
    NSObjectFileImage           image;
    NSObjectFileImageReturnCode result;
    dl_dyld = malloc(sizeof *dl_dyld);
    if (dl_dyld != NULL) {
        dl_dyld->base.impl = &li_dl_impl_dyld;
        dl_dyld->module    = NULL;
        dl_dyld->image     = NULL;
        result             = NSCreateObjectFileImageFromFile(name, &image);
        if (result == NSObjectFileImageSuccess) {
            dl_dyld->module =
                NSLinkModule(image, name, NSLINKMODULE_OPTION_RETURN_ON_ERROR);
            NSDestroyObjectFileImage(image);
            if (dl_dyld->module != NULL) {
                return dl_dyld;
            }
        } else if (result == NSObjectFileImageInappropriateFile) {
            dl_dyld->image = NSAddImage(
                name, NSADDIMAGE_OPTION_RETURN_ON_ERROR
                          | NSADDIMAGE_OPTION_WITH_SEARCHING);
            if (dl_dyld->image != NULL) {
                return dl_dyld;
            }
        }
        free(dl_dyld);
    }
    return NULL;
}

static int li_dl_close_dyld(li_dl_t dl) {
    struct li_dl_dyld *dl_dyld = dl;
    bool               result  = false;
    if (dl_dyld->module != NULL) {
        result = NSUnLinkModule(dl_dyld->module, NSUNLINKMODULE_OPTION_NONE);
    }
    free(dl_dyld);
    return result ? 0 : -1;
}

static li_dl_sym_t li_dl_sym_dyld(li_dl_t dl, const char *name) {
    struct li_dl_dyld *dl_dyld = dl;
    NSSymbol           symbol  = NULL;
    char              *symbol_name;
    symbol_name = malloc(strlen(name) + 2);
    if (symbol_name != NULL) {
        symbol_name[0] = '_';
        strcpy(symbol_name + 1, name);
        if (dl_dyld->module != NULL) {
            symbol = NSLookupSymbolInModule(dl_dyld->module, symbol_name);
        } else if (dl_dyld->image != NULL) {
            symbol = NSLookupSymbolInImage(
                dl_dyld->image, symbol_name,
                NSLOOKUPSYMBOLINIMAGE_OPTION_BIND
                    | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR);
        }
        free(symbol_name);
        if (symbol != NULL) {
            return NSAddressOfSymbol(symbol);
        }
    }
    return NULL;
}

static li_dl_fun_t li_dl_fun_dyld(li_dl_t dl, const char *name) {
    union li_dl_symbol symbol;
    symbol.sym = li_dl_sym_dyld(dl, name);
    return symbol.fun;
}
#endif

li_dl_t li_dl_open(const char *name) {
    li_dl_t dl;
#if LI_DL_DLFCN
    dl = li_dl_open_dlfcn(name);
    if (dl != NULL) {
        return dl;
    }
#endif
#if LI_DL_WIN32
    dl = li_dl_open_win32(name);
    if (dl != NULL) {
        return dl;
    }
#endif
#if LI_DL_DYLD
    dl = li_dl_open_dyld(name);
    if (dl != NULL) {
        return dl;
    }
#endif
    return NULL;
}

int li_dl_close(li_dl_t dl) {
    struct li_dl_base *dl_base = dl;
    return dl_base->impl->close(dl);
}

li_dl_sym_t li_dl_sym(li_dl_t dl, const char *name) {
    struct li_dl_base *dl_base = dl;
    return dl_base->impl->sym(dl, name);
}

li_dl_fun_t li_dl_fun(li_dl_t dl, const char *name) {
    struct li_dl_base *dl_base = dl;
    return dl_base->impl->fun(dl, name);
}
