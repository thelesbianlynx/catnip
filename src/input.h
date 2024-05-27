#pragma once

#include "main.h"

enum input_status {
    INPUT_NONE,

    INPUT_CHAR,
    INPUT_ALT_CHAR,
    INPUT_CTRL_CHAR,

    INPUT_TAB,
    INPUT_ENTER,
    INPUT_ESC,
    INPUT_BACKSPACE,

    INPUT_SHIFT_TAB,
    INPUT_ALT_ENTER,
    INPUT_ALT_BACKSPACE,

    INPUT_UP,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_RIGHT,

    INPUT_SHIFT_UP,
    INPUT_SHIFT_DOWN,
    INPUT_SHIFT_LEFT,
    INPUT_SHIFT_RIGHT,

    INPUT_ALT_UP,
    INPUT_ALT_DOWN,
    INPUT_ALT_LEFT,
    INPUT_ALT_RIGHT,

    INPUT_SHIFT_ALT_UP,
    INPUT_SHIFT_ALT_DOWN,
    INPUT_SHIFT_ALT_LEFT,
    INPUT_SHIFT_ALT_RIGHT,

    INPUT_CTRL_UP,
    INPUT_CTRL_DOWN,
    INPUT_CTRL_LEFT,
    INPUT_CTRL_RIGHT,

    INPUT_SHIFT_CTRL_UP,
    INPUT_SHIFT_CTRL_DOWN,
    INPUT_SHIFT_CTRL_LEFT,
    INPUT_SHIFT_CTRL_RIGHT,

    INPUT_HOME,
    INPUT_END,

    INPUT_PGUP,
    INPUT_PGDOWN,

    INPUT_INSERT,
    INPUT_DELETE,

    INPUT_F1,
    INPUT_F2,
    INPUT_F3,
    INPUT_F4,
    INPUT_F5,
    INPUT_F6,
    INPUT_F7,
    INPUT_F8,
    INPUT_F9,
    INPUT_F10,
    INPUT_F11,
    INPUT_F12,

    INPUT_MOUSE_PRESS,
    INPUT_MOUSE_RELEASE,
    INPUT_MOUSE_MOTION,
};

struct input_state {
    int32_t charcode;
    int32_t x, y;
};

uint32_t nextkey (int32_t timeout, struct input_state* r_inputstate, uint32_t* debug);
