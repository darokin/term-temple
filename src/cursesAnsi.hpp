#ifndef TERMTEMPLE_CURSESANSI_H
#define TERMTEMPLE_CURSESANSI_H

#ifdef __unix__
    #include <cursesw.h>
#elif defined _WIN32
    #define NCURSES_WIDECHAR 1
    #include <ncursesw/ncurses.h>
#endif

#define ASCII_ESC           '\x1B'
#define ASCII_EOF           '\x1A'
#define ASCII_BELL          '\x07'
#define ASCII_BACKSPACE     '\x08'
#define ASCII_HORIZONTALTAB '\x09'
#define ASCII_LINEFEED      '\x0A'
#define ASCII_VERTICALTAB   '\x0B'
#define ASCII_FORMFEED      '\x0C'
#define ASCII_CARRIAGERETURN '\x0D'
#define ASCII_DELETE        '\x7F'

#define MOD_START            '['
#define MOD_COMPLEX_2_UNDO   '2'
#define MOD_COMPLEX_3_FG     '3'
#define MOD_COMPLEX_4_BG     '4'

#define MOD_0_RESET     '0'
#define MOD_1_BOLD      '1'
#define MOD_2_DIM       '2'
#define MOD_3_ITALIC    '3'
#define MOD_4_UNDERLINE '4'
#define MOD_5_BLINK     '5'

#define MOD_7_REVERSE   '7'
#define MOD_8_HIDDEN    '8'
#define MOD_9_STRIKE    '9'

#define MOD_END         'm'
#define MOD_SPLITTER    ';'

#define ASCII_DIGIT_START   48

#define ANSI_COLORSTART_FG  30
#define ANSI_COLORSTART_BG  40

#define ANSI_COLOR_BLACK	0
#define ANSI_COLOR_RED		1
#define ANSI_COLOR_GREEN	2
#define ANSI_COLOR_YELLOW	3
#define ANSI_COLOR_BLUE		4
#define ANSI_COLOR_MAGENTA	5
#define ANSI_COLOR_CYAN		6
#define ANSI_COLOR_WHITE	7
#define ANSI_COLOR_COUNT	8

namespace cursesANSI {
    bool mvwaddnANSIstr(WINDOW* _win, int _startY, int _startX, std::string _line);
	bool mvwaddnANSIstr(int _startY, int _startX, std::string _line);
    uint16_t getANSIstrLength(std::string _line);
    unsigned char feedChar(unsigned char c, WINDOW* _win);
};

#endif