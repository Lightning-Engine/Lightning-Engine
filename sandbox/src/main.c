#include "sandbox.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    li_dl_t dl;
    void (*puts)(const char *);
    dl = li_dl_open("libc.so.6");
    if (dl == NULL) {
        printf("%s\n", li_dl_error());
        dl = li_dl_open("msvcrt.dll");
    }
    if (dl == NULL) {
        printf("%s\n", li_dl_error());
        dl = li_dl_open("libSystem.dylib");
    }
    if (dl == NULL) {
        printf("%s\n", li_dl_error());
    } else {
        puts = (void (*)(const char *)) li_dl_fun(dl, "puts");
        if (puts != NULL) {
            puts("Hello, World!");
        }
        li_dl_close(dl);
    }
    return EXIT_SUCCESS;
}
