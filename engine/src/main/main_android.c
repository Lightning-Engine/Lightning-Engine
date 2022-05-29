#include "li/main/android.h"

void android_main(struct android_app *state) {
    li_main_android_app = state;
    li_main();
}
