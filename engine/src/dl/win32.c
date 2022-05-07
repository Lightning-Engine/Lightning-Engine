#include "li/dl/win32.h"

#include "li/std.h"

#ifndef LI_DL_WIN32_MAX_MERROR
# define LI_DL_WIN32_MAX_MERROR 256
#endif

union li_dl_symbol {
    li_dl_sym_t sym;
    li_dl_fun_t fun;
};

static char li_dl_win32_error_str[LI_DL_WIN32_MAX_MERROR];

const struct li_dl_impl li_dl_win32_impl = { li_dl_win32_close, li_dl_win32_sym,
                                             li_dl_win32_fun };

li_dl_t li_dl_win32_open(const char *name) {
    struct li_dl_win32 *dl_win32;
    dl_win32 = li_std_malloc(sizeof *dl_win32);
    if (dl_win32 != NULL) {
        dl_win32->base.impl = &li_dl_win32_impl;
        if (name == NULL) {
            dl_win32->module = GetModuleHandle(NULL);
        } else {
            dl_win32->module = LoadLibrary(name);
        }
        if (dl_win32->module != NULL) {
            return (li_dl_t) dl_win32;
        }
        li_dl_win32_error();
        li_std_free(dl_win32);
    }
    return NULL;
}

void li_dl_win32_close(li_dl_t dl) {
    struct li_dl_win32 *dl_win32 = (struct li_dl_win32 *) dl;
    FreeLibrary(dl_win32->module);
    li_std_free(dl_win32);
}

li_dl_sym_t li_dl_win32_sym(li_dl_t dl, const char *name) {
    union li_dl_symbol symbol;
    symbol.fun = li_dl_win32_fun(dl, name);
    return symbol.sym;
}

li_dl_fun_t li_dl_win32_fun(li_dl_t dl, const char *name) {
    struct li_dl_win32 *dl_win32 = (struct li_dl_win32 *) dl;
    FARPROC             fun;
    fun = GetProcAddress(dl_win32->module, name);
    if (fun != NULL) {
        return (li_dl_fun_t) fun;
    }
    li_dl_win32_error();
    return NULL;
}

void li_dl_win32_error(void) {
    FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK, NULL,
        GetLastError(), MAKELANGID(LANG_SYSTEM_DEFAULT, LANG_USER_DEFAULT),
        li_dl_win32_error_str, LI_DL_WIN32_MAX_MERROR, NULL);
    li_dl_error_str = li_dl_win32_error_str;
}
