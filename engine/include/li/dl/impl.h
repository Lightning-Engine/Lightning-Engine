#ifndef LI_DL_IMPL_H
#define LI_DL_IMPL_H

#include "li/dl/dl.h"

union li_dl_symbol {
    li_dl_sym_t sym;
    li_dl_fun_t fun;
};

struct li_dl_impl {
    li_dl_t (*open)(const char *name);
    void (*close)(li_dl_t dl);
    li_dl_sym_t (*sym)(li_dl_t dl, const char *name);
    li_dl_fun_t (*fun)(li_dl_t dl, const char *name);
};

struct li_dl_base {
    const struct li_dl_impl *impl;
};

extern const struct li_dl_impl li_dl_dlfcn_impl;
extern const struct li_dl_impl li_dl_win32_impl;
extern const struct li_dl_impl li_dl_dyld_impl;

void li_dl_set_error(const char *str);

#endif
