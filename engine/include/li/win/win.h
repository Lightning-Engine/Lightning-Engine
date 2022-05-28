#ifndef LI_WIN_WIN_H
#define LI_WIN_WIN_H

#include "li/input.h"

/**
 * @file
 * @brief Window creation.
 * @note This interface must be used from the main thread.
 *
 * @code
 * li_win_key_fun    = key_fun;
 * li_win_button_fun = button_fun;
 * li_win_motion_fun = motion_fun;
 * li_win_size_fun   = size_fun;
 * li_win_close_fun  = close_fun;
 * li_win_init();
 * win = li_win_create(640, 480);
 * while (running) {
 *     li_win_poll();
 * }
 * li_win_destroy(win);
 * li_win_exit();
 * @endcode
 */

enum li_win_msg {
    li_win_msg_key_down,
    li_win_msg_key_up,
    li_win_msg_key_repeat,
    li_win_msg_button_down,
    li_win_msg_button_up,
    li_win_msg_motion,
    li_win_msg_size,
    li_win_msg_close
};

typedef enum li_win_msg li_win_msg_t;
typedef struct li_win  *li_win_t;

typedef void (*li_key_fun_t)(
    li_win_t win, li_win_msg_t msg, li_input_state_t state, li_input_key_t key);
typedef void (*li_button_fun_t)(
    li_win_t win, li_win_msg_t msg, li_input_state_t state, int x, int y,
    li_input_button_t button);
typedef void (*li_motion_fun_t)(
    li_win_t win, li_win_msg_t msg, li_input_state_t state, int x, int y);
typedef void (*li_size_fun_t)(
    li_win_t win, li_win_msg_t msg, int width, int height);
typedef void (*li_close_fun_t)(li_win_t win, li_win_msg_t msg);

extern li_key_fun_t    li_win_key_fun;
extern li_button_fun_t li_win_button_fun;
extern li_motion_fun_t li_win_motion_fun;
extern li_size_fun_t   li_win_size_fun;
extern li_close_fun_t  li_win_close_fun;

/**
 * @brief Initialize the window system.
 * @return 0 on success, -1 on failure.
 */
int      li_win_init(void);
/**
 * @brief Exit the window system.
 */
void     li_win_exit(void);
/**
 * @brief Poll the window system.
 */
void     li_win_poll(void);
/**
 * @brief Create a window.
 * @param width The width of the window.
 * @param height The height of the window.
 * @return The window on success, NULL on failure.
 */
li_win_t li_win_create(int width, int height);
/**
 * @brief Destroy a window.
 * @param win The window to destroy.
 */
void     li_win_destroy(li_win_t win);

#endif
