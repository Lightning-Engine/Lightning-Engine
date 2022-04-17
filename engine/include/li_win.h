/**
 * @file li_win.h
 * @brief Windowing library.
 * @note This interface must be used from the main thread.
 */

#ifndef LI_WIN_H
#define LI_WIN_H

typedef void *li_win_t;
typedef void (*li_win_fun_t)(void);

int      li_win_init(li_win_fun_t fun);
void     li_win_poll(void);
li_win_t li_win_create(int width, int height);
void     li_win_destroy(li_win_t win);

#endif
