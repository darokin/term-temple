#include <ctime>
#include <vector>

#include "screen.hpp"
#include "renderer.hpp"
#include "widget.hpp"
#include "widgetClock.hpp"

#define CHARBIG_COLS        5
#define CHARBIG_LINES       11

#define CHARSMALL_COLS      3
#define CHARSMALL_LINES     11

std::wstring arrNum[CHARBIG_LINES][CHARBIG_COLS] = { 
	{L"████████", L"██⠀⠀⠀⠀██", L"██⠀⠀⠀⠀██", L"██⠀⠀⠀⠀██", L"████████"},
	{L"██", L"██", L"██", L"██", L"██"},
	{L"████████", L"⠀⠀⠀⠀⠀⠀██", L"████████", L"██      ", L"████████"},
	{L"████████", L"⠀⠀⠀⠀⠀⠀██", L"████████", L"⠀⠀⠀⠀⠀⠀██", L"████████"},
	{L"██⠀⠀⠀⠀██", L"██⠀⠀⠀⠀██", L"████████", L"⠀⠀⠀⠀⠀⠀██", L"⠀⠀⠀⠀⠀⠀██"},
	{L"████████", L"██⠀⠀⠀⠀", L"████████", L"⠀⠀⠀⠀⠀⠀██", L"████████"},
	{L"████████", L"██⠀⠀⠀⠀", L"████████", L"██⠀⠀⠀⠀██", L"████████"},
	{L"████████", L"⠀⠀⠀⠀⠀⠀██", L"⠀⠀⠀⠀⠀⠀██", L"⠀⠀⠀⠀⠀⠀██", L"⠀⠀⠀⠀⠀⠀██"},
	{L"████████", L"██⠀⠀⠀⠀██", L"████████", L"██⠀⠀⠀⠀██", L"████████"},
	{L"████████", L"██⠀⠀⠀⠀██", L"████████", L"⠀⠀⠀⠀⠀⠀██", L"████████"},
	{L"⠀⠀⠀", L"███", L"⠀⠀⠀", L"███", L"⠀⠀⠀"}
 };

// # SMALL ASCII DIGITS
std::wstring arrNumSmall[CHARSMALL_LINES][CHARSMALL_COLS] = { 
	{L"▄▄▄▄", L"█⠀⠀█", L"█▄▄█"},
	{L"▄", L"█", L"█"},
	{L"▄▄▄▄", L"▄▄▄█", L"█▄▄▄"},
	{L"▄▄▄▄", L"⠀▄▄█", L"▄▄▄█"},
	{L"▄⠀⠀▄", L"█▄▄█", L"⠀⠀⠀█"},
	{L"▄▄▄▄", L"█▄▄▄", L"▄▄▄█"},
	{L"▄▄▄▄", L"█▄▄▄", L"█▄▄█"},
	{L"▄▄▄▄", L"⠀⠀⠀█", L"⠀⠀⠀█"},
	{L"▄▄▄▄", L"█▄▄█", L"█▄▄█"},
	{L"▄▄▄▄", L"█▄▄█", L"▄▄▄█"},
	{L"⠀", L"■", L"■"}
};

// TODO compartiment in namespace ?
inline static void drawNumber(int _num, int _posx, int _posy, bool _isBig) {
    int nbLines = (_isBig ? CHARBIG_COLS : CHARSMALL_COLS); // STUPID c'est reverse le nommage des constantes...

    for (int i = 0; i < nbLines; i++) {
        std::wstring* wstr = (_isBig ? &arrNum[_num][i] : &arrNumSmall[_num][i]);
        renderer::drawString(wstr->c_str(), {_posx, _posy});
        _posy = _posy + 1;
    }
};

// ========================================================

WidgetClock::WidgetClock(const std::wstring& _title, i2d _pos, i2d _size) : Widget::Widget(_title, _pos, _size) {
}
WidgetClock::WidgetClock(const std::wstring& _title) : Widget::Widget(_title) {
}
WidgetClock::~WidgetClock() {
}

void WidgetClock::handleKey(int _keycode) {
    if (_keycode == 27 || _keycode == 9)
        return;
    this->isBig = !this->isBig;
};

void WidgetClock::draw() {
    char hourMinute[6];

    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    snprintf(hourMinute, 6, "%02d:%02d", now->tm_hour, now->tm_min);

    // == Parcours pour calcul du centrage;
    int val;
    int sizeX = 0;
    int sizeY = (this->isBig ? CHARBIG_COLS : CHARSMALL_COLS);
    for (int i = 0; i < 5; i++) {
        val = (int)hourMinute[i] - 48;  // Lucky me ':' is ascii char 59 right after '9' :)
        if (this->isBig)
            sizeX += (arrNum[val][0].length() + 2);
        else 
            sizeX += (arrNumSmall[val][0].length() + 1);
    }
    // == Center start in win
    int start_x = (this->size.x / 2.0) - (sizeX / 2) + this->pos.x;
    int start_y = (this->size.y / 2.0) - (sizeY / 2) + this->pos.y - 1;
    
    // == Parcours pour affichage
    int curX = start_x;
    for (int i = 0; i < 5; i++) {
        val = (int)hourMinute[i] - 48;
        drawNumber(val, curX, start_y, this->isBig);
        if (this->isBig)
            curX += (arrNum[val][0].length() + 2);
        else 
            curX += (arrNumSmall[val][0].length() + 1);
    }

    renderer::setColorReverse(true);
    char strDate[32];
    snprintf(strDate, 32, " %02d/%02d/%hu ", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
    renderer::drawString(strDate, {start_x + sizeX - (this->isBig ? 14 : 13), start_y + sizeY});
    renderer::setColorReverse(false);
}
