#include "li/main/main.h"

#include <android_native_app_glue.h>

void android_main(struct android_app *state) {
    (void) state;
    li_main();
}
