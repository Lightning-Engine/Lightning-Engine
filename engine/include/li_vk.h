#ifndef LI_VK_H
#define LI_VK_H

#include "li_win.h"

typedef struct li_vkctx li_vkctx_t;

li_vkctx_t *li_vkctx_create(li_win_t *win, int version);
int         li_vkctx_make_current(li_win_t *win, li_vkctx_t *ctx);
int         li_vkctx_swap_buffers(li_win_t *win, li_vkctx_t *ctx);
int         li_vkctx_destroy(li_win_t *win, li_vkctx_t *ctx);
void       *li_vkctx_get_proc_addr(const char *name);

#endif
