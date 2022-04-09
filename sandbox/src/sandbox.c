#include "sandbox.h"

#include <stddef.h>
#include <stdio.h>

int main(void) {
    void *handle              = NULL;
    int (*puts)(const char *) = NULL;

    if (!(handle = li_dlopen("libc.so.6"))
        && !(handle = li_dlopen("ucrtbase.dll"))) {
        fprintf(stderr, "dlopen failed\n");
        return EXIT_FAILURE;
    }

    if (!(*(void **) &puts = li_dlsym(handle, "puts"))) {
        printf("Failed to find strlen\n");
        return EXIT_FAILURE;
    }

    puts("Hello, World!");
    return EXIT_SUCCESS;
}
