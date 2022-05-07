#include "li/dl/dlfcn.h"

#include "li/std.h"

union li_dl_symbol {
    li_dl_sym_t sym;
    li_dl_fun_t fun;
};

const struct li_dl_impl li_dl_dlfcn_impl = { li_dl_dlfcn_close, li_dl_dlfcn_sym,
                                             li_dl_dlfcn_fun };

li_dl_t li_dl_dlfcn_open(const char *name) {
    struct li_dl_dlfcn *dl_dlfcn;
    dl_dlfcn = li_std_malloc(sizeof *dl_dlfcn);
    if (dl_dlfcn != NULL) {
        dl_dlfcn->base.impl = &li_dl_dlfcn_impl;
        dl_dlfcn->handle    = dlopen(name, RTLD_LAZY);
        if (dl_dlfcn->handle != NULL) {
            return (li_dl_t) dl_dlfcn;
        }
        li_dl_dlfcn_error();
        li_std_free(dl_dlfcn);
    }
    return NULL;
}

void li_dl_dlfcn_close(li_dl_t dl) {
    struct li_dl_dlfcn *dl_dlfcn = (struct li_dl_dlfcn *) dl;
    dlclose(dl_dlfcn->handle);
    li_std_free(dl_dlfcn);
}

li_dl_sym_t li_dl_dlfcn_sym(li_dl_t dl, const char *name) {
    struct li_dl_dlfcn *dl_dlfcn = (struct li_dl_dlfcn *) dl;
    void               *sym;
    sym = dlsym(dl_dlfcn->handle, name);
    if (sym != NULL) {
        return sym;
    }
    li_dl_dlfcn_error();
    return NULL;
}

li_dl_fun_t li_dl_dlfcn_fun(li_dl_t dl, const char *name) {
    union li_dl_symbol symbol;
    symbol.sym = li_dl_dlfcn_sym(dl, name);
    return symbol.fun;
}

void li_dl_dlfcn_error(void) {
    li_dl_error_str = dlerror();
}
