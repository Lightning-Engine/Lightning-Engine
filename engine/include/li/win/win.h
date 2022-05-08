#ifndef LI_WIN_WIN_H
#define LI_WIN_WIN_H

#include "li/key.h"

/**
 * @file
 * @brief Windowing library.
 * @note This interface must be used from the main thread.
 */

enum li_win_msg {
    li_win_msg_keydown,
    li_win_msg_keyup,
    li_win_msg_keyrep,
    li_win_msg_mousedown,
    li_win_msg_mouseup,
    li_win_msg_mousemove,
    li_win_msg_resize,
    li_win_msg_close
};

typedef enum li_win_msg li_win_msg_t;
typedef struct li_win  *li_win_t;
typedef void (*li_key_fun_t)(
    li_win_t win, li_win_msg_t action, li_key_code_t key, li_key_state_t state);
typedef void (*li_mouse_fun_t)(
    li_win_t win, li_win_msg_t action, li_key_code_t key, int x, int y,
    li_key_state_t state);
typedef void (*li_resize_fun_t)(
    li_win_t win, li_win_msg_t action, int width, int height);
typedef void (*li_close_fun_t)(li_win_t win, li_win_msg_t action);

struct li_win {
    void *ptr;
    int   width;
    int   height;
};

extern li_key_fun_t    li_win_key_fun;
extern li_mouse_fun_t  li_win_mouse_fun;
extern li_resize_fun_t li_win_resize_fun;
extern li_close_fun_t  li_win_close_fun;

int      li_win_init(void);
void     li_win_exit(void);
void     li_win_poll(void);
li_win_t li_win_create(int width, int height);
void     li_win_destroy(li_win_t win);

#endif
