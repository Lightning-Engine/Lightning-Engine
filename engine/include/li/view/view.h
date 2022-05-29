#ifndef LI_VIEW_VIEW_H
#define LI_VIEW_VIEW_H

enum li_view_msg {
    li_view_key_msg,
    li_view_motion_msg
};

typedef enum li_view_msg li_view_msg_t;

typedef void (*li_view_key_fun_t)(li_view_msg_t msg);
typedef void (*li_view_motion_fun_t)(li_view_msg_t msg);

extern li_view_key_fun_t    li_view_key_fun;
extern li_view_motion_fun_t li_view_motion_fun;

int  li_view_init(void);
void li_view_poll(void);

#endif
