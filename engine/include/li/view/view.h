#ifndef LI_VIEW_VIEW_H
#define LI_VIEW_VIEW_H

#include "li/input.h"

enum li_view_msg {
    li_view_msg_key_down,
    li_view_msg_key_up,
    li_view_msg_key_repeat,
    li_view_msg_touch_down,
    li_view_msg_touch_up,
    li_view_msg_touch_move
};

typedef enum li_view_msg li_view_msg_t;

typedef void (*li_view_key_fun_t)(
    li_view_msg_t msg, li_input_state_t state, li_input_key_t key);
typedef void (*li_view_touch_fun_t)(
    li_view_msg_t msg, li_input_state_t state, int x, int y, int id);

extern li_view_key_fun_t   li_view_key_fun;
extern li_view_touch_fun_t li_view_touch_fun;

int  li_view_init(void);
void li_view_poll(void);
void li_view_show_kb(int show);

#endif
