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

    void tick() { 
        currentTimeInMs = Utils::timeInMilliseconds(); 
    };

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
};
