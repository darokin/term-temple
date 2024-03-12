#ifndef TERMTEMPLE_SCREEN_H
#define TERMTEMPLE_SCREEN_H

#include "utils.hpp"
#include "widgetsManager.hpp"

#ifndef CURSES // include for headers that do not include ncurses
#define KEY_DOWN        258
#define KEY_UP          259
#define KEY_LEFT        260
#define KEY_RIGHT       261
#define KEY_ENTER       343
#define KEY_BACKSPACE   363
#define KEY_ESCAPE      27
#define KEY_F0          264
#define KEY_F1          265
#define KEY_F2          266
#define KEY_F3          267
#define KEY_F4          268
#define KEY_F5          269
#define KEY_F6          270
#define KEY_F7          271
#define KEY_F8          272
#define KEY_F9          273
#define KEY_F10         274
#define KEY_F11         275
#define KEY_F12         276
#define KEY_F(x)        ((KEY_F0) + (x))
#define KEY_HOME        262
#define KEY_TAB         0x09
#endif
#define KEY_SPACE       ' '

enum colorPairs {
    DEFAULT,
    WHITE_ON_BLACK,
    BLACK_ON_WHITE,
    YELLOW_ON_WHITE,
    YELLOW_ON_BLACK,
    YELLOW_ON_BLUE,
    BLACK_ON_YELLOW,
    PINK_ON_WHITE,
    PINK_ON_BLACK,
    BLACK_ON_PINK,
    BLUE_ON_WHITE,
    BLUE_ON_BLACK,
    BLUE_ON_YELLOW,
    BLACK_ON_BLUE
};

// == CURSES COLORS ==============================================
inline static uint16_t getColor(uint16_t _colorPair) {
    return _colorPair;//(999 - _colorPair); // <- must keep, seem useless but maybe usefull for monochrome 'hack' for example
}

void screenInit();
void screenLoop();
void screenExit();
void screenResize();
void statusRefresh();
void initWidgets();
void drawWidgets();

//void click1(Module* _module, Widget* _widget, i2d _pos);

#endif