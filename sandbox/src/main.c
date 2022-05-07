#include "sandbox.h"

#include <stdio.h>
#include <stdlib.h>

void win_fun(li_win_msg_t msg) {
    printf("win_fun\n");
    if (msg == li_win_msg_close) {
        li_win_destroy(li_win_win);
        li_win_exit();
        exit(EXIT_SUCCESS);
    }
}

int main(void) {
    li_win_t win;
    li_win_fun = win_fun;
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
