#ifndef TERMTEMPLE_RENDERER_H
#define TERMTEMPLE_RENDERER_H

#ifdef __unix__
    #include <cursesw.h>
#elif // defined _WIN32
    #define NCURSES_WIDECHAR 1
    #include <ncursesw/ncurses.h>
#endif

#include "utils.hpp"

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