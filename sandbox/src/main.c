#include "sandbox.h"

#include <stdio.h>
#include <stdlib.h>

void key_fun(
    li_win_t win, li_win_msg_t action, li_key_code_t key,
    li_key_state_t state) {
    (void) win;
    (void) action;
    (void) key;
    (void) state;
    printf("key event\n");
}

void mouse_fun(
    li_win_t win, li_win_msg_t action, li_key_code_t key, int x, int y,
    li_key_state_t state) {
    (void) win;
    (void) action;
    (void) key;
    (void) x;
    (void) y;
    (void) state;
    printf("mouse event\n");
}

void resize_fun(li_win_t win, li_win_msg_t action, int width, int height) {
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
    li_win_mouse_fun  = mouse_fun;
    li_win_resize_fun = resize_fun;
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
