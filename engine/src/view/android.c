#include "li/view/android.h"

const struct li_view_impl li_view_android_impl = {
    li_view_android_poll,
};

void li_view_android_poll(void) {
    struct android_input_buffer *input;
}
