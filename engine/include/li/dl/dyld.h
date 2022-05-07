#ifndef LI_DL_DYLD_H
#define LI_DL_DYLD_H

#include "li/dl/dl.h"

#include <mach-o/dyld.h>

struct li_dl_dyld {
    struct li_dl              base;
    NSModule                  module;
    const struct mach_header *image;
};

extern const struct li_dl_impl li_dl_dyld_impl;

li_dl_t     li_dl_dyld_open(const char *name);
void        li_dl_dyld_close(li_dl_t dl);
li_dl_sym_t li_dl_dyld_sym(li_dl_t dl, const char *name);
li_dl_fun_t li_dl_dyld_fun(li_dl_t dl, const char *name);
void        li_dl_dyld_error(void);

#endif
