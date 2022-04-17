#include "sandbox.h"

#include <stdio.h>
#include <stdlib.h>

void win_fun(void) {
    printf("win_fun\n");
}

int main(void) {
    int      i;
    li_win_t win;
    li_win_init(win_fun);
    win = li_win_create(640, 480);
    for (i = 0; i < 1000000; i++) {
        li_win_poll();
    }
    li_win_destroy(win);
    return EXIT_SUCCESS;
}
