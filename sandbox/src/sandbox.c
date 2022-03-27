#include "sandbox.h"

#include <stddef.h>
#include <stdio.h>

int main(void) {
    void   *ptr;
    li_dl_t dl;
    size_t (*strlen_ptr)(const char *);

    if (li_dl_open(&dl, "ucrtbase.dll")) {
        if (li_dl_open(&dl, "libc.so.6")) {
            printf("Failed to open libc\n");
            return EXIT_FAILURE;
        }
    }

    ptr = li_dl_sym(&dl, "strlen");

    if (ptr == NULL) {
        printf("Failed to find strlen\n");
        return EXIT_FAILURE;
    }

    *(void **) &strlen_ptr = ptr;

    printf("%u\n", (unsigned) strlen_ptr("Hello, world!"));

    return EXIT_SUCCESS;
}
