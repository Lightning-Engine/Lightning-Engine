#include "li/dl/impl.h"

#include "li/std.h"

static const char *li_dl_error_str = NULL;

li_dl_t li_dl_open(const char *name) {
    li_dl_t dl = NULL;
    li_dl_set_error(NULL);
#if LI_DL_DLFCN
    if (dl == NULL) {
        dl = li_dl_dlfcn_impl.open(name);
    }
#endif
#if LI_DL_WIN32
    if (dl == NULL) {
        dl = li_dl_win32_impl.open(name);
    }
#endif
#if LI_DL_DYLD
    if (dl == NULL) {
        dl = li_dl_dyld_impl.open(name);
    }
#endif
    return dl;
}

void li_dl_close(li_dl_t dl) {
    struct li_dl_base *dl_base = (struct li_dl_base *) dl;
    li_dl_set_error(NULL);
    dl_base->impl->close(dl);
}

li_dl_sym_t li_dl_sym(li_dl_t dl, const char *name) {
    struct li_dl_base *dl_base = (struct li_dl_base *) dl;
    li_dl_set_error(NULL);
    return dl_base->impl->sym(dl, name);
}

li_dl_fun_t li_dl_fun(li_dl_t dl, const char *name) {
    struct li_dl_base *dl_base = (struct li_dl_base *) dl;
    li_dl_set_error(NULL);
    return dl_base->impl->fun(dl, name);
}

const char *li_dl_error(void) {
    return li_dl_error_str;
}

void li_dl_set_error(const char *str) {
    li_dl_error_str = str;
}
