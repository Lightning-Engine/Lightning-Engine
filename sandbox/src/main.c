#include "sandbox.h"

#include <stdio.h>
#include <stdlib.h>

static int running = 1;

void key_fun(
    li_win_t win, li_win_msg_t msg, li_input_state_t state,
    li_input_key_t key) {
    (void) win;
    if (msg == li_win_msg_key_down) {
        printf("key_down    %04hx %04hx\n", state, key);
    } else if (msg == li_win_msg_key_repeat) {
        printf("key_repeat  %04hx %04hx\n", state, key);
    } else {
        printf("key_up      %04hx %04hx\n", state, key);
    }
}

void button_fun(
    li_win_t win, li_win_msg_t msg, li_input_state_t state, int x, int y,
    li_input_button_t button) {
    (void) win;
    if (msg == li_win_msg_button_down) {
        printf("button_down %04hx %4d %4d %04hx\n", state, x, y, button);
    } else {
        printf("button_up   %04hx %4d %4d %04hx\n", state, x, y, button);
    }
}

void motion_fun(
    li_win_t win, li_win_msg_t msg, li_input_state_t state, int x, int y) {
    (void) win;
    (void) msg;
    printf("motion      %04hx %4d %4d\n", state, x, y);
}

void size_fun(li_win_t win, li_win_msg_t msg, int width, int height) {
    (void) win;
    (void) msg;
    printf("size        %4d %4d\n", width, height);
}

void close_fun(li_win_t win, li_win_msg_t msg) {
    (void) win;
    (void) msg;
    printf("close\n");
    running = 0;
}

void li_main(void) {
    li_win_t win;
    li_win_key_fun    = key_fun;
    li_win_button_fun = button_fun;
    li_win_motion_fun = motion_fun;
    li_win_size_fun   = size_fun;
    li_win_close_fun  = close_fun;
    li_win_init();
    win = li_win_create(640, 480);
    while (running) {
        li_win_poll();
    }
    li_win_destroy(win);
    li_win_exit();
}
