/**
 * @file li_win.h
 * @brief Windowing library.
 * @since 0.0.1
 * @note This library must be used from the main thread
 */

#ifndef LI_WIN_H
#define LI_WIN_H

typedef struct li_win li_win_t;
typedef void (*li_win_fun_t)(li_event_t *);

typedef struct li_win_hints {
    int         width;
    int         height;
    const char *title;
} li_win_hints_t;

typedef union li_event {
} li_event_t;

int li_win_init(li_win_fun_t cb);
int li_win_exit(void);
int li_win_poll(void);

void      li_win_hints_init(li_win_hints_t *hints);
li_win_t *li_win_create(li_win_hints_t *hints);
int       li_win_destroy(li_win_t *win);

int   li_win_set_data(li_win_t *win, void *data);
void *li_win_get_data(li_win_t *win);

int li_win_set_vis(li_win_t *win, int vis);

#endif
