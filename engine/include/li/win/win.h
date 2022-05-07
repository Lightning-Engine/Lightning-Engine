#ifndef LI_WIN_WIN_H
#define LI_WIN_WIN_H

/**
 * @file
 * @brief Windowing library.
 * @note This interface must be used from the main thread.
 */

#define LI_WIN_KEY_0        0x0000
#define LI_WIN_KEY_1        0x0001
#define LI_WIN_KEY_2        0x0002
#define LI_WIN_KEY_3        0x0003
#define LI_WIN_KEY_4        0x0004
#define LI_WIN_KEY_5        0x0005
#define LI_WIN_KEY_6        0x0006
#define LI_WIN_KEY_7        0x0007
#define LI_WIN_KEY_8        0x0008
#define LI_WIN_KEY_9        0x0009
#define LI_WIN_KEY_A        0x000A
#define LI_WIN_KEY_B        0x000B
#define LI_WIN_KEY_C        0x000C
#define LI_WIN_KEY_D        0x000D
#define LI_WIN_KEY_E        0x000E
#define LI_WIN_KEY_F        0x000F
#define LI_WIN_KEY_G        0x0010
#define LI_WIN_KEY_H        0x0011
#define LI_WIN_KEY_I        0x0012
#define LI_WIN_KEY_J        0x0013
#define LI_WIN_KEY_K        0x0014
#define LI_WIN_KEY_L        0x0015
#define LI_WIN_KEY_M        0x0016
#define LI_WIN_KEY_N        0x0017
#define LI_WIN_KEY_O        0x0018
#define LI_WIN_KEY_P        0x0019
#define LI_WIN_KEY_Q        0x001A
#define LI_WIN_KEY_R        0x001B
#define LI_WIN_KEY_S        0x001C
#define LI_WIN_KEY_T        0x001D
#define LI_WIN_KEY_U        0x001E
#define LI_WIN_KEY_V        0x001F
#define LI_WIN_KEY_W        0x0020
#define LI_WIN_KEY_X        0x0021
#define LI_WIN_KEY_Y        0x0022
#define LI_WIN_KEY_Z        0x0023
#define LI_WIN_KEY_NUM0     0x0100
#define LI_WIN_KEY_NUM1     0x0101
#define LI_WIN_KEY_NUM2     0x0102
#define LI_WIN_KEY_NUM3     0x0103
#define LI_WIN_KEY_NUM4     0x0104
#define LI_WIN_KEY_NUM5     0x0105
#define LI_WIN_KEY_NUM6     0x0106
#define LI_WIN_KEY_NUM7     0x0107
#define LI_WIN_KEY_NUM8     0x0108
#define LI_WIN_KEY_NUM9     0x0109
#define LI_WIN_KEY_NUMDOT   0x010A
#define LI_WIN_KEY_NUMPLUS  0x010B
#define LI_WIN_KEY_NUMMINUS 0x010C
#define LI_WIN_KEY_NUMSTAR  0x010D
#define LI_WIN_KEY_NUMSLASH 0x010E
#define LI_WIN_KEY_NUMENTER 0x010F
#define LI_WIN_KEY_NUMLOCK  0x0110
#define LI_WIN_KEY_F1       0x0111
#define LI_WIN_KEY_F2       0x0112
#define LI_WIN_KEY_F3       0x0113
#define LI_WIN_KEY_F4       0x0114
#define LI_WIN_KEY_F5       0x0115
#define LI_WIN_KEY_F6       0x0116
#define LI_WIN_KEY_F7       0x0117
#define LI_WIN_KEY_F8       0x0118
#define LI_WIN_KEY_F9       0x0119
#define LI_WIN_KEY_F10      0x011A
#define LI_WIN_KEY_F11      0x011B
#define LI_WIN_KEY_F12      0x011C
#define LI_WIN_KEY_PRINTSCR 0x0200
#define LI_WIN_KEY_SCRLOCK  0x0201
#define LI_WIN_KEY_PAUSE    0x0202
#define LI_WIN_KEY_INSERT   0x0203
#define LI_WIN_KEY_HOME     0x0204
#define LI_WIN_KEY_PAGEUP   0x0205
#define LI_WIN_KEY_DELETE   0x0206
#define LI_WIN_KEY_END      0x0207
#define LI_WIN_KEY_PAGEDOWN 0x0208
#define LI_WIN_KEY_UP       0x0209
#define LI_WIN_KEY_DOWN     0x020A
#define LI_WIN_KEY_LEFT     0x020B
#define LI_WIN_KEY_RIGHT    0x020C
#define LI_WIN_KEY_ESCAPE   0x020D
#define LI_WIN_KEY_BSPACE   0x020E
#define LI_WIN_KEY_TAB      0x020F
#define LI_WIN_KEY_LSHIFT   0x0210
#define LI_WIN_KEY_RSHIFT   0x0211
#define LI_WIN_KEY_LCONTROL 0x0212
#define LI_WIN_KEY_RCONTROL 0x0213
#define LI_WIN_KEY_LSUPER   0x0214
#define LI_WIN_KEY_RSUPER   0x0215
#define LI_WIN_KEY_LALT     0x0216
#define LI_WIN_KEY_RALT     0x0217
#define LI_WIN_KEY_CAPSLOCK 0x0218
#define LI_WIN_KEY_ENTER    0x0219
#define LI_WIN_KEY_SPACE    0x0300
#define LI_WIN_KEY_DOT      0x0301
#define LI_WIN_KEY_COMMA    0x0302
#define LI_WIN_KEY_COLON    0x0303
#define LI_WIN_KEY_QUOTE    0x0304
#define LI_WIN_KEY_LBRACKET 0x0305
#define LI_WIN_KEY_RBRACKET 0x0306
#define LI_WIN_KEY_BSLASH   0x0307
#define LI_WIN_KEY_MINUS    0x0308
#define LI_WIN_KEY_SLASH    0x0309
#define LI_WIN_KEY_TILDE    0x030A
#define LI_WIN_KEY_EQUAL    0x030B
#define LI_WIN_KEY_LMOUSE   0x1000
#define LI_WIN_KEY_RMOUSE   0x1001
#define LI_WIN_KEY_MMOUSE   0x1002
#define LI_WIN_KEY_NULL     0xFFFF

#define LI_WIN_STATE_SHIFT    0x0001
#define LI_WIN_STATE_CAPSLOCK 0x0002
#define LI_WIN_STATE_CONTROL  0x0004
#define LI_WIN_STATE_ALT      0x0008
#define LI_WIN_STATE_NUMLOCK  0x0010
#define LI_WIN_STATE_SUPER    0x0020
#define LI_WIN_STATE_LMOUSE   0x0100
#define LI_WIN_STATE_RMOUSE   0x0200
#define LI_WIN_STATE_MMOUSE   0x0400

enum li_win_msg {
    li_win_msg_keydown,
    li_win_msg_keyup,
    li_win_msg_keyrep,
    li_win_msg_mousedown,
    li_win_msg_mouseup,
    li_win_msg_mousemove,
    li_win_msg_resize,
    li_win_msg_close
};

typedef unsigned short  li_win_key_t;
typedef unsigned short  li_win_state_t;
typedef enum li_win_msg li_win_msg_t;
typedef struct li_win  *li_win_t;
typedef void (*li_win_fun_t)(li_win_msg_t msg);

struct li_win_impl {
    void (*exit)(void);
    void (*poll)(void);
    li_win_t (*create)(int width, int height);
    void (*destroy)(li_win_t win);
};

struct li_win {
    void          *ptr;
    int            width;
    int            height;
    int            mousex;
    int            mousey;
    li_win_state_t state;
};

extern const struct li_win_impl *li_win_impl;
extern li_win_fun_t              li_win_fun;
extern li_win_t                  li_win_win;
extern li_win_key_t              li_win_key;

int      li_win_init(void);
void     li_win_exit(void);
void     li_win_poll(void);
li_win_t li_win_create(int width, int height);
void     li_win_destroy(li_win_t win);

#endif
