#ifndef LI_DL_WIN32_H
#define LI_DL_WIN32_H

#include "li/dl/dl.h"

#include <windows.h>

struct li_dl_win32 {
    struct li_dl base;
    HMODULE      module;
};

extern const struct li_dl_impl li_dl_win32_impl;

li_dl_t     li_dl_win32_open(const char *name);
void        li_dl_win32_close(li_dl_t dl);
li_dl_sym_t li_dl_win32_sym(li_dl_t dl, const char *name);
li_dl_fun_t li_dl_win32_fun(li_dl_t dl, const char *name);
void        li_dl_win32_error(void);

#endif
