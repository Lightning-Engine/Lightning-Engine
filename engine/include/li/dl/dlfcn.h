#ifndef LI_DL_DLFCN_H
#define LI_DL_DLFCN_H

#include "li/dl/impl.h"

#include <dlfcn.h>

struct li_dl_dlfcn {
    struct li_dl_base base;
    void             *handle;
};

li_dl_t     li_dl_dlfcn_open(const char *name);
void        li_dl_dlfcn_close(li_dl_t dl);
li_dl_sym_t li_dl_dlfcn_sym(li_dl_t dl, const char *name);
li_dl_fun_t li_dl_dlfcn_fun(li_dl_t dl, const char *name);
void        li_dl_dlfcn_error(void);

#endif
