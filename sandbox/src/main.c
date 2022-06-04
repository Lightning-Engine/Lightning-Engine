#include "sandbox.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef ANDROID
# include <android/log.h>
# define LOG(...) __android_log_print(ANDROID_LOG_INFO, "li", __VA_ARGS__)
#else
# define LOG(...) printf(__VA_ARGS__)
#endif

static int running = 1;

void win_key_fun(
    li_win_t win, li_win_msg_t msg, li_input_state_t state,
    li_input_key_t key) {
    (void) win;
    if (msg == li_win_msg_key_down) {
        LOG("key_down    %04hx %04hx\n", state, key);
    } else if (msg == li_win_msg_key_repeat) {
        LOG("key_repeat  %04hx %04hx\n", state, key);
    } else {
        LOG("key_up      %04hx %04hx\n", state, key);
    }
}

void win_button_fun(
    li_win_t win, li_win_msg_t msg, li_input_state_t state, int x, int y,
    li_input_button_t button) {
    (void) win;
    if (msg == li_win_msg_button_down) {
        LOG("button_down %04hx %4d %4d %04hx\n", state, x, y, button);
    } else {
        LOG("button_up   %04hx %4d %4d %04hx\n", state, x, y, button);
    }
}

void win_motion_fun(
    li_win_t win, li_win_msg_t msg, li_input_state_t state, int x, int y) {
    (void) win;
    (void) msg;
    LOG("motion      %04hx %4d %4d\n", state, x, y);
}

void win_size_fun(li_win_t win, li_win_msg_t msg, int width, int height) {
    (void) win;
    (void) msg;
    LOG("size        %4d %4d\n", width, height);
}

void win_close_fun(li_win_t win, li_win_msg_t msg) {
    (void) win;
    (void) msg;
    LOG("close\n");
    running = 0;
}

void view_key_fun(
    li_view_msg_t msg, li_input_state_t state, li_input_key_t key) {
    if (msg == li_view_msg_key_down) {
        LOG("key_down    %04hx %04hx", state, key);
    } else if (msg == li_view_msg_key_repeat) {
        LOG("key_repeat  %04hx %04hx", state, key);
    } else if (msg == li_view_msg_key_up) {
        LOG("key_up      %04hx %04hx", state, key);
    }
}

void view_touch_fun(
    li_view_msg_t msg, li_input_state_t state, int x, int y, int id) {
    if (msg == li_view_msg_touch_down) {
        LOG("touch_down  %04hx %4d %4d %4d", state, x, y, id);
    } else if (msg == li_view_msg_touch_up) {
        LOG("touch_up    %04hx %4d %4d %4d", state, x, y, id);
    } else {
        LOG("touch_move  %04hx %4d %4d %4d", state, x, y, id);
    }
}

void li_main(void) {
    li_win_t win;
    li_win_key_fun    = win_key_fun;
    li_win_button_fun = win_button_fun;
    li_win_motion_fun = win_motion_fun;
    li_win_size_fun   = win_size_fun;
    li_win_close_fun  = win_close_fun;
    li_view_key_fun   = view_key_fun;
    li_view_touch_fun = view_touch_fun;
    LOG("li_main\n");
    if (li_win_init() == 0) {
        win = li_win_create(640, 480);
        while (running) {
            li_win_poll();
        }
        li_win_destroy(win);
        li_win_exit();
    } else if (li_view_init() == 0) {
        while (1)
            li_view_poll();
    }
}
