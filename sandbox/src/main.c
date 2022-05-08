#include "sandbox.h"

#include <stdio.h>
#include <stdlib.h>

void key_fun(
    li_win_t win, li_win_msg_t msg, li_input_state_t state,
    li_input_key_t key) {
    (void) win;
    (void) msg;
    (void) state;
    (void) key;
    printf("key event\n");
}

void button_fun(
    li_win_t win, li_win_msg_t msg, li_input_state_t state, int x, int y,
    li_input_button_t button) {
    (void) win;
    (void) msg;
    (void) state;
    (void) x;
    (void) y;
    (void) button;
    printf("button event\n");
}

void motion_fun(
    li_win_t win, li_win_msg_t msg, li_input_state_t state, int x, int y) {
    (void) win;
    (void) msg;
    (void) state;
    (void) x;
    (void) y;
    printf("motion event\n");
}

void size_fun(li_win_t win, li_win_msg_t action, int width, int height) {
    (void) win;
    (void) action;
    (void) width;
    (void) height;
    printf("resize event\n");
}

void close_fun(li_win_t win, li_win_msg_t action) {
    (void) win;
    (void) action;
    printf("close event\n");
    li_win_destroy(win);
    li_win_exit();
    exit(EXIT_SUCCESS);
}

int main(void) {
    li_win_t win;
    li_win_key_fun    = key_fun;
    li_win_button_fun = button_fun;
    li_win_motion_fun = motion_fun;
    li_win_size_fun   = size_fun;
    li_win_close_fun  = close_fun;
    li_win_init();
    win = li_win_create(640, 480);
    if (win == NULL)
        fprintf(stderr, "An error ocurred when creating a window\n");
    while (1)
        li_win_poll();
    li_win_destroy(win);
    li_win_exit();
    return EXIT_SUCCESS;
}
