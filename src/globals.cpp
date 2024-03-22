#include <cstdarg>
#include "globals.hpp"

#define STATUS_TEXT_MAXLENGTH	256

namespace globals {
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
    GameState gameState {GameState::INGAME_START};
    long long currentTimeInMs;
    wchar_t statusText[STATUS_TEXT_MAXLENGTH];
    i2d termSize;

    // == CURSES COLORS ==============================================
    uint16_t getColor(uint16_t _colorPair) {
        return _colorPair; //(999 - _colorPair); // <- must keep, seem useless but maybe usefull for monochrome 'hack' for example
    }

    // == TIME TICK ===================================================
    void tick() { 
        currentTimeInMs = Utils::timeInMilliseconds(); 
    }

    // == CENTRALIZED STATUS MSG =======================================
    // TODO : migrate in widgetManager() ?
    int setStatusText(const wchar_t* _line, ...) {
        va_list vl;
        va_start(vl, _line);
        auto ret = vswprintf(statusText, STATUS_TEXT_MAXLENGTH, _line, vl);
        va_end(vl);
        return ret;
    }

    const wchar_t* getStatusText() {
        return statusText;
    }
}
