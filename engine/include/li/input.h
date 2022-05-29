#ifndef LI_INPUT_H
#define LI_INPUT_H

/**
 * @file
 * @brief Input definitions.
 */

#define LI_INPUT_STATE_SHIFT    0x0001
#define LI_INPUT_STATE_CAPSLOCK 0x0002
#define LI_INPUT_STATE_CONTROL  0x0004
#define LI_INPUT_STATE_ALT      0x0008
#define LI_INPUT_STATE_NUMLOCK  0x0010
#define LI_INPUT_STATE_SUPER    0x0020
#define LI_INPUT_STATE_LMB      0x0100
#define LI_INPUT_STATE_RMB      0x0200
#define LI_INPUT_STATE_MMB      0x0400

#define LI_INPUT_KEY_0        0x0000
#define LI_INPUT_KEY_1        0x0001
#define LI_INPUT_KEY_2        0x0002
#define LI_INPUT_KEY_3        0x0003
#define LI_INPUT_KEY_4        0x0004
#define LI_INPUT_KEY_5        0x0005
#define LI_INPUT_KEY_6        0x0006
#define LI_INPUT_KEY_7        0x0007
#define LI_INPUT_KEY_8        0x0008
#define LI_INPUT_KEY_9        0x0009
#define LI_INPUT_KEY_A        0x000A
#define LI_INPUT_KEY_B        0x000B
#define LI_INPUT_KEY_C        0x000C
#define LI_INPUT_KEY_D        0x000D
#define LI_INPUT_KEY_E        0x000E
#define LI_INPUT_KEY_F        0x000F
#define LI_INPUT_KEY_G        0x0010
#define LI_INPUT_KEY_H        0x0011
#define LI_INPUT_KEY_I        0x0012
#define LI_INPUT_KEY_J        0x0013
#define LI_INPUT_KEY_K        0x0014
#define LI_INPUT_KEY_L        0x0015
#define LI_INPUT_KEY_M        0x0016
#define LI_INPUT_KEY_N        0x0017
#define LI_INPUT_KEY_O        0x0018
#define LI_INPUT_KEY_P        0x0019
#define LI_INPUT_KEY_Q        0x001A
#define LI_INPUT_KEY_R        0x001B
#define LI_INPUT_KEY_S        0x001C
#define LI_INPUT_KEY_T        0x001D
#define LI_INPUT_KEY_U        0x001E
#define LI_INPUT_KEY_V        0x001F
#define LI_INPUT_KEY_W        0x0020
#define LI_INPUT_KEY_X        0x0021
#define LI_INPUT_KEY_Y        0x0022
#define LI_INPUT_KEY_Z        0x0023
#define LI_INPUT_KEY_NUM0     0x0100
#define LI_INPUT_KEY_NUM1     0x0101
#define LI_INPUT_KEY_NUM2     0x0102
#define LI_INPUT_KEY_NUM3     0x0103
#define LI_INPUT_KEY_NUM4     0x0104
#define LI_INPUT_KEY_NUM5     0x0105
#define LI_INPUT_KEY_NUM6     0x0106
#define LI_INPUT_KEY_NUM7     0x0107
#define LI_INPUT_KEY_NUM8     0x0108
#define LI_INPUT_KEY_NUM9     0x0109
#define LI_INPUT_KEY_NUMDOT   0x010A
#define LI_INPUT_KEY_NUMPLUS  0x010B
#define LI_INPUT_KEY_NUMMINUS 0x010C
#define LI_INPUT_KEY_NUMSTAR  0x010D
#define LI_INPUT_KEY_NUMSLASH 0x010E
#define LI_INPUT_KEY_NUMENTER 0x010F
#define LI_INPUT_KEY_NUMLOCK  0x0110
#define LI_INPUT_KEY_ESCAPE   0x0200
#define LI_INPUT_KEY_F1       0x0201
#define LI_INPUT_KEY_F2       0x0202
#define LI_INPUT_KEY_F3       0x0203
#define LI_INPUT_KEY_F4       0x0204
#define LI_INPUT_KEY_F5       0x0205
#define LI_INPUT_KEY_F6       0x0206
#define LI_INPUT_KEY_F7       0x0207
#define LI_INPUT_KEY_F8       0x0208
#define LI_INPUT_KEY_F9       0x0209
#define LI_INPUT_KEY_F10      0x020A
#define LI_INPUT_KEY_F11      0x020B
#define LI_INPUT_KEY_F12      0x020C
#define LI_INPUT_KEY_PRINTSCR 0x0300
#define LI_INPUT_KEY_SCRLOCK  0x0301
#define LI_INPUT_KEY_PAUSE    0x0302
#define LI_INPUT_KEY_INSERT   0x0303
#define LI_INPUT_KEY_HOME     0x0304
#define LI_INPUT_KEY_PAGEUP   0x0305
#define LI_INPUT_KEY_DELETE   0x0306
#define LI_INPUT_KEY_END      0x0307
#define LI_INPUT_KEY_PAGEDOWN 0x0308
#define LI_INPUT_KEY_UP       0x0309
#define LI_INPUT_KEY_DOWN     0x030A
#define LI_INPUT_KEY_LEFT     0x030B
#define LI_INPUT_KEY_RIGHT    0x030C
#define LI_INPUT_KEY_BSPACE   0x0400
#define LI_INPUT_KEY_TAB      0x0401
#define LI_INPUT_KEY_LSHIFT   0x0402
#define LI_INPUT_KEY_RSHIFT   0x0403
#define LI_INPUT_KEY_LCONTROL 0x0404
#define LI_INPUT_KEY_RCONTROL 0x0405
#define LI_INPUT_KEY_LSUPER   0x0406
#define LI_INPUT_KEY_RSUPER   0x0407
#define LI_INPUT_KEY_LALT     0x0408
#define LI_INPUT_KEY_RALT     0x0409
#define LI_INPUT_KEY_CAPSLOCK 0x040A
#define LI_INPUT_KEY_ENTER    0x040B
#define LI_INPUT_KEY_SPACE    0x0500
#define LI_INPUT_KEY_DOT      0x0501
#define LI_INPUT_KEY_COMMA    0x0502
#define LI_INPUT_KEY_COLON    0x0503
#define LI_INPUT_KEY_QUOTE    0x0504
#define LI_INPUT_KEY_LBRACKET 0x0505
#define LI_INPUT_KEY_RBRACKET 0x0506
#define LI_INPUT_KEY_BSLASH   0x0507
#define LI_INPUT_KEY_MINUS    0x0508
#define LI_INPUT_KEY_SLASH    0x0509
#define LI_INPUT_KEY_TILDE    0x050A
#define LI_INPUT_KEY_EQUAL    0x050B
#define LI_INPUT_KEY_NULL     0xFFFF

#define LI_INPUT_BUTTON_LEFT   0x0000
#define LI_INPUT_BUTTON_RIGHT  0x0001
#define LI_INPUT_BUTTON_MIDDLE 0x0002
#define LI_INPUT_BUTTON_UP     0x0003
#define LI_INPUT_BUTTON_DOWN   0x0004
#define LI_INPUT_BUTTON_NULL   0xFFFF

typedef unsigned short li_input_state_t;
typedef unsigned short li_input_key_t;
typedef unsigned short li_input_button_t;

#endif
