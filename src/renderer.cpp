#include "renderer.hpp"

// TODO : refactor drawString with template

namespace renderer {

    void setColor(uint8_t _colorPair) {
        wattron(stdscr, COLOR_PAIR(_colorPair) | A_BOLD);   
    }

    void setColorReverse(bool _reverse) {
        if (_reverse)
            wattron(stdscr, A_REVERSE);
        else
            wattroff(stdscr, A_REVERSE);
    }

    // == WIDE CHAR
    void drawString(const wchar_t* _line) {
        waddwstr(stdscr, _line);
    }

    void drawString(const wchar_t* _line, uint16_t _size) {
        waddnwstr(stdscr, _line, _size);
    }

    void drawString(const wchar_t* _line, i2d _pos) {
        wmove(stdscr, _pos.y, _pos.x);
        drawString(_line);
    }

    void drawString(const wchar_t* _line, i2d _pos, uint16_t _size) {
        wmove(stdscr, _pos.y, _pos.x);
        drawString(_line, _size);
    }

    // == CHAR (NARROW / ASCII)
    void drawString(const char* _line) {
        waddnstr(stdscr, _line, -1);
    }

    void drawString(const char* _line, uint16_t _size) {
        waddnstr(stdscr, _line, _size);
    }

    void drawString(const char* _line, i2d _pos) {
        wmove(stdscr, _pos.y, _pos.x);
        drawString(_line);
    }

    void drawString(const char* _line, i2d _pos, uint16_t _size) {
        wmove(stdscr, _pos.y, _pos.x);
        drawString(_line, _size);
    }
}
