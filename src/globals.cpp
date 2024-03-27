#include <cstdarg>
#include "globals.hpp"

namespace globals {
    // == GLOBAL CONSTANTS ===========================================
    const wchar_t* cursBlock    = L"█";
    const wchar_t* cursBlock2   = L"▓";
    const wchar_t* cursBlock3   = L"▒";
    const wchar_t* longSpacesLine = L"\
                                                \
                                                \
                                                \
                                                \
                                                \
                                                ";

    // == GLOBAL VARIABLES ===========================================
    GameState gameState {GameState::INGAME_START};
    long long currentTimeInMs {};
    i2d termSize {};

    // ===============================================================
    // == CURSES COLORS ==============================================
    uint16_t getColor(uint16_t _colorPair) {
        return _colorPair; //(999 - _colorPair); // <- must keep, seem useless but maybe usefull for monochrome 'hack' for example
    }

    // == TIME TICK ===================================================
    void tick() { 
        currentTimeInMs = Utils::timeInMilliseconds(); 
    }
}
