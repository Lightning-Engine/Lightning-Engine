#include "sandbox.h"

#include <stdlib.h>

int main(void) {
    li_dl_t *handle;
    void (*puts)(const char *);
    if (!li_dl_init()) {
        if ((handle = li_dl_open("libc.so.6"))) {
            if ((puts = (void (*)(const char *)) li_dl_fsym(handle, "puts"))) {
                puts("Hello, World!");
            }
            li_dl_close(handle);
        }
    }
    return EXIT_SUCCESS;
}
