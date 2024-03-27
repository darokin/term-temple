#ifndef TERMTEMPLE_GLOBALS_H
#define TERMTEMPLE_GLOBALS_H

#include "utils.hpp"

/*
#ifndef CURSES // include for headers that do not include ncurses
#define KEY_F0          264
#define KEY_F(x)        ((KEY_F0) + (x))
#define KEY_MOUSE       409
#define KEY_RESIZE      410
#endif
*/

enum colorPairs {
    COLORPAIR_DEFAULT,
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
    BLACK_ON_BLUE,
    COLORPAIR_END
};

enum class GameState {
    BOOTLOADING,
    INGAME_START,
    INGAME_MOVE,
    INGAME_RESIZE,
    INGAME_SNAP,
    INGAME_END,
    INANIMATION
};

namespace globals {
    // == GLOBAL VARIABLES
    extern long long currentTimeInMs;
    extern GameState gameState;
    extern i2d termSize;

    // == GLOBAL FUNCTIONS
    void tick();
    uint16_t getColor(uint16_t _colorPair);

    // == GLOBAL CONSTANTS
    extern const wchar_t* cursBlock;
    extern const wchar_t* cursBlock2;
    extern const wchar_t* cursBlock3;
    extern const wchar_t* longSpacesLine;
    
    // == KEY CONSTANTS
    namespace keys {
        const uint16_t KeyDown          = 258;
        const uint16_t KeyUp            = 259;
        const uint16_t KeyLeft          = 260;
        const uint16_t KeyRight         = 261;
        const uint16_t KeyEnter         = 13;
        const uint16_t KeyEnterNumPad   = 343;
        const uint16_t KeyBackspace     = 363;
        const uint16_t KeyEscape        = 27;
        const uint16_t KeyF0            = 264;
        const uint16_t KeyF1            = 265;
        const uint16_t KeyF2            = 266;
        const uint16_t KeyF3            = 267;
        const uint16_t KeyF4            = 268;
        const uint16_t KeyF5            = 269;
        const uint16_t KeyF6            = 270;
        const uint16_t KeyF7            = 271;
        const uint16_t KeyF8            = 272;
        const uint16_t KeyF9            = 273;
        const uint16_t KeyF10           = 274;
        const uint16_t KeyF11           = 275;
        const uint16_t KeyF12           = 276;
        const uint16_t KeyHome          = 262;
        const uint16_t KeyTab           = 0x09;
        const uint16_t KeySpace         = ' ';
        const uint16_t KeyPageDown      = 338;
        const uint16_t KeyPageUp        = 339;
        const uint16_t KeyShiftLeft     = 393;
        const uint16_t KeyShiftRight    = 402;
    }
}

#endif