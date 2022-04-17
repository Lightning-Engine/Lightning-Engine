#include "sandbox.h"

#include <stdio.h>
#include <stdlib.h>

void win_fun(void) {
    printf("win_fun\n");
}

int main(void) {
    li_win_init(win_fun);
    li_win_create(640, 480);
    while (1) {
        li_win_poll();
    }
    return EXIT_SUCCESS;
}
