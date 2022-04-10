#ifndef LI_GL_H
#define LI_GL_H

#include "li_win.h"

typedef struct li_glctx li_glctx_t;

li_glctx_t *li_glctx_create(li_win_t *win, int version);
int         li_glctx_make_current(li_win_t *win, li_glctx_t *ctx);
int         li_glctx_swap_buffers(li_win_t *win, li_glctx_t *ctx);
int         li_glctx_destroy(li_win_t *win, li_glctx_t *ctx);
void       *li_glctx_get_proc_addr(const char *name);

#endif
