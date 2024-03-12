#ifndef TERMTEMPLE_RENDERER_H
#define TERMTEMPLE_RENDERER_H

// TODO __linux__ / __APPLE__ / __WIN32
#if defined (__unix__) || (__linux__)
    #include <cursesw.h>
#else
    #define NCURSES_WIDECHAR 1
    #include <ncursesw/ncurses.h>
#endif

#include "utils.hpp"

// TODO refactor

namespace renderer {
	void setColor(uint8_t _colorPair);
	void setColorReverse(bool _reverse);
	// Wide char
	void drawString(const wchar_t* _line);
	void drawString(const wchar_t* _line, uint16_t _size);
	void drawString(const wchar_t* _line, i2d _pos);
	void drawString(const wchar_t* _line, i2d _pos, uint16_t _size);
	// Narrow char
	void drawString(const char* _line);
	void drawString(const char* _line, uint16_t _size);
	void drawString(const char* _line, i2d _pos);
	void drawString(const char* _line, i2d _pos, uint16_t _size);
};

#endif