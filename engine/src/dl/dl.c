#include "li/dl/dl.h"

#include "li/std.h"

#if LI_DL_DLFCN
# include "li/dl/dlfcn.h"
#endif
#if LI_DL_WIN32
# include "li/dl/win32.h"
#endif
#if LI_DL_DYLD
# include "li/dl/dyld.h"
#endif

const char *li_dl_error_str = NULL;

li_dl_t li_dl_open(const char *name) {
    li_dl_t dl      = NULL;
    li_dl_error_str = NULL;
#if LI_DL_DLFCN
    if (dl == NULL) {
        dl = li_dl_dlfcn_open(name);
    }
#endif
#if LI_DL_WIN32
    if (dl == NULL) {
        dl = li_dl_win32_open(name);
    }
#endif
#if LI_DL_DYLD
    if (dl == NULL) {
        dl = li_dl_dyld_open(name);
    }
#endif
    return dl;
}

void li_dl_close(li_dl_t dl) {
    li_dl_error_str = NULL;
    dl->impl->close(dl);
}

li_dl_sym_t li_dl_sym(li_dl_t dl, const char *name) {
    li_dl_error_str = NULL;
    return dl->impl->sym(dl, name);
}

li_dl_fun_t li_dl_fun(li_dl_t dl, const char *name) {
    li_dl_error_str = NULL;
    return dl->impl->fun(dl, name);
}

const char *li_dl_error(void) {
    return li_dl_error_str;
}
