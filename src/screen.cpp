#include <iostream>
#include <sstream>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
    #define NCURSES_WIDECHAR 1
    #include <ncursesw/ncurses.h>
    #define msleep(x) Sleep((x))
#else // __unix__ / __linux__ / __APPLE__
    #include <unistd.h> // usleep()
    #include <ncursesw/curses.h>
    #include <ncursesw/menu.h>
    #define msleep(x) usleep((x) * 100)
#endif

const std::string backgroundAnsiPath = "../data/ans/eyes_pretty_02.ans"; // ok path for win also
//const std::string backgroundAnsiPath = "../data/ans/terry_01.ans";

#include "screen.hpp"
#include "widgetsManager.hpp"
#include "widget.hpp"
#include "widgetClock.hpp"
#include "widgetTextFile.hpp"
#include "widgetANSI.hpp"
#include "widgetDialog.hpp"
#include "widgetMsgbox.hpp"
#include "widgetAppLauncher.hpp"
#include "module.hpp"
#include "moduleScroller.hpp"
#include "moduleOneLiner.hpp"
#include "utils.hpp"
#include "globals.hpp"

WidgetManager* wmgr;
WINDOW *wstatus; // <-- remove and put module inside wmgr instead

// == We define BRIGHT colors outise of the range of 'normal' curses colors
#define COLOR_DEC_CONST	        32
#define COLOR_BRIGHT_BLUE       (COLOR_DEC_CONST + 9)
#define COLOR_BRIGHT_MAGENTA    (COLOR_DEC_CONST + 13)
#define COLOR_BRIGHT_YELLOW     (COLOR_DEC_CONST + 14)
#define COLOR_BRIGHT_WHITE      (COLOR_DEC_CONST + 15)

// == Init widgets
void initWidgets() {
    wmgr = WidgetManager::getInstance(); // TODO remove useless singleton
    wmgr->setBackground(backgroundAnsiPath);
    wmgr->alert(L"""Press <SPACE BAR> or <Q> to validate this message and then press <F2> to open the APP LAUNCHER.\n\
I suggest you go first use the TUTORIAL.\nUse <SPACE BAR> to launch an application.\nHAVE FUN!""", false);
    
    /*
    WidgetDialog* _wd = WidgetDialog::createDialogPtrWithFile("../data/txt/terry01.txt");
    _wd->setPos({68, 58});
    _wd->setSpeed(200);
    wmgr->addWidget(_wd);
   */
}

/*
void click1(Module* _module, Widget* _widget, i2d _pos) {
    // TODO : size parameter to alert (small / medium / big)
    wmgr->alert(L"CLICKED");
}
*/

// == TODO : move as a module inside widgetManager
void drawStatusBar() {
    werase(wstatus);
    wbkgd(wstatus, COLOR_PAIR(globals::getColor(colorPairs::BLUE_ON_WHITE)) | A_BOLD | A_REVERSE);
    wmove(wstatus, 0, 0);
    wprintw(wstatus, "%ls", globals::getStatusText());
    wrefresh(wstatus);
}

void screenInit() {
    // == Init screen
    initscr();
    wclear(stdscr);
    cbreak();               // one char at a time (do not buffer till enter)
    curs_set(FALSE);        // hide cursor
    noecho();               // suppress echoing of keys input
    keypad(stdscr, TRUE);   // allow getch() to listen to arrows and other special keys
    nodelay(stdscr, TRUE);  // non blocking getch()
    nonl();                 // no new line / catch enter key

    // == Init color pairs
    //if (has_colors()) {   // TODO : handle monochrome avec un flag et hack getColor() for same return (?)
        use_default_colors();
        start_color();
    //};
    use_default_colors();
    if (can_change_color() && COLORS >= 16) {
        init_color(COLOR_BRIGHT_WHITE, 1000, 1000, 1000);
        init_color(COLOR_BRIGHT_MAGENTA, 1000, 0, 0);
        init_color(COLOR_BRIGHT_BLUE, 0, 0, 1000);
        init_color(COLOR_BRIGHT_YELLOW, 1000, 1000, 0);

        init_pair(globals::getColor(colorPairs::BLACK_ON_WHITE), COLOR_BLACK, COLOR_BRIGHT_WHITE);
        init_pair(globals::getColor(colorPairs::YELLOW_ON_WHITE), COLOR_YELLOW, COLOR_BRIGHT_WHITE);
        init_pair(globals::getColor(colorPairs::PINK_ON_WHITE), COLOR_MAGENTA, COLOR_BRIGHT_WHITE);	
        init_pair(globals::getColor(colorPairs::BLUE_ON_WHITE), COLOR_BLUE, COLOR_BRIGHT_WHITE);
        init_pair(globals::getColor(colorPairs::BLUE_ON_YELLOW), COLOR_BLUE, COLOR_BRIGHT_YELLOW);
        init_pair(globals::getColor(colorPairs::BLACK_ON_YELLOW), COLOR_BLACK, COLOR_BRIGHT_YELLOW);
        init_pair(globals::getColor(colorPairs::YELLOW_ON_BLUE), COLOR_YELLOW, COLOR_BRIGHT_BLUE);
        init_pair(globals::getColor(colorPairs::BLACK_ON_BLUE), COLOR_BLACK, COLOR_BRIGHT_BLUE);
        init_pair(globals::getColor(colorPairs::BLACK_ON_PINK), COLOR_BLACK, COLOR_BRIGHT_MAGENTA);	
    } else {
        init_pair(globals::getColor(colorPairs::BLACK_ON_WHITE), COLOR_BLACK, COLOR_WHITE);
        init_pair(globals::getColor(colorPairs::YELLOW_ON_WHITE), COLOR_YELLOW, COLOR_WHITE);
        init_pair(globals::getColor(colorPairs::PINK_ON_WHITE), COLOR_MAGENTA, COLOR_WHITE);	
        init_pair(globals::getColor(colorPairs::BLUE_ON_WHITE), COLOR_BLUE, COLOR_WHITE);
        init_pair(globals::getColor(colorPairs::BLUE_ON_YELLOW), COLOR_BLUE, COLOR_YELLOW);
        init_pair(globals::getColor(colorPairs::BLACK_ON_YELLOW), COLOR_BLACK, COLOR_YELLOW);
        init_pair(globals::getColor(colorPairs::YELLOW_ON_BLUE), COLOR_YELLOW, COLOR_BLUE);
        init_pair(globals::getColor(colorPairs::BLACK_ON_BLUE), COLOR_BLACK, COLOR_BLUE);
        init_pair(globals::getColor(colorPairs::BLACK_ON_PINK), COLOR_BLACK, COLOR_MAGENTA);	
    }
    init_pair(colorPairs::COLORPAIR_DEFAULT, -1, -1);
    init_pair(globals::getColor(colorPairs::WHITE_ON_BLACK), COLOR_WHITE, COLOR_BLACK);
    init_pair(globals::getColor(colorPairs::YELLOW_ON_BLACK), COLOR_YELLOW, COLOR_BLACK);
    init_pair(globals::getColor(colorPairs::PINK_ON_BLACK), COLOR_MAGENTA, COLOR_BLACK);	
    init_pair(globals::getColor(colorPairs::BLUE_ON_BLACK), COLOR_BLUE, COLOR_BLACK);

    // == Mouse events
    mousemask(ALL_MOUSE_EVENTS, NULL);

    // == Init term size by faking a resize
    screenResize();

    // == Create main wmaindow and initialize default style
    //wMain = newwmain(viewSize.y, viewSize.x, 0, 0);
    wstatus = newwin(1, globals::termSize.x, globals::termSize.y - 1, 0);

    // == Init global time (do it once so widget constructors get their timeStart)
    globals::tick();

    // == Init widgets
    initWidgets();
}

void screenLoop() {
    bool bQuit = false;
    int keycode;
    MEVENT event;

    do {
        msleep(80);
        
        // ============================================
        globals::tick();
        wmgr->draw();
        drawStatusBar();
        wrefresh(stdscr);
        // ============================================

        // == KEY LISTENING
        keycode = wgetch(stdscr);
        if (keycode == -1)
            continue;

        // == HEY HANDLING
        switch (keycode) {
            case KEY_MOUSE:
                if(getmouse(&event) != OK)
                    break;
                if(event.bstate & BUTTON1_PRESSED) {	
                    wmgr->handleMousePressed({event.x, event.y});
                } else if(event.bstate & REPORT_MOUSE_POSITION) {	
                    wmgr->handleMouseMove({event.x, event.y});
                } else if(event.bstate & BUTTON1_RELEASED) {	
                    wmgr->handleMouseReleased({event.x, event.y});
                } else if(event.bstate & BUTTON1_CLICKED) {	
                    wmgr->handleMouseClicked({event.x, event.y});
                }	
                break;
            case KEY_RESIZE:
                screenResize();
                wmgr->refreshWidgets(globals::termSize.x, globals::termSize.y);
                break;
            case KEY_F(2):
                wmgr->toggleAppLauncher();
                break;
            case 'M':
            case 'm':
                if (globals::gameState == GameState::INGAME_START) {
                    globals::gameState = GameState::INGAME_MOVE;
                    globals::setStatusText(L"[MOVE] %02d", 42);
                }
                break;
            case 'S':
            case 's':
                if (globals::gameState == GameState::INGAME_START) {
                    globals::gameState = GameState::INGAME_SNAP;
                    globals::setStatusText(L"[SNAP] %c", 'X');
                }
                break;
            case 'R':
            case 'r':
                if (globals::gameState == GameState::INGAME_START) {
                    globals::gameState = GameState::INGAME_RESIZE;
                    globals::setStatusText(L"[RESIZE]");
                }
                break;
            case 27: // ESCAPE
                if (globals::gameState > GameState::INGAME_START && globals::gameState < GameState::INGAME_END) {
                    // == Quit any special mode (resize, move, snap, etc.)
                    globals::gameState = GameState::INGAME_START;
                    globals::setStatusText(L"");
                } else if (globals::gameState == GameState::INGAME_START) {
                    bQuit = true;
                }
                drawStatusBar();
                break;
            default:
                switch(globals::gameState) {
                    case GameState::INGAME_MOVE:
                        wmgr->handleMove(keycode);
                        break;
                    case GameState::INGAME_RESIZE:
                        wmgr->handleResize(keycode);
                        break;
                    case GameState::INGAME_SNAP:
                        wmgr->handleSnap(keycode);
                        break;
                    default:
                        wmgr->handleKey(keycode);
                }
                globals::setStatusText(L"Key '%d' '0x%x' [%c]", keycode, keycode, (char)keycode);
                //wrefresh(wstatus);
        }
    } while(!bQuit);
}

void statusRefresh() {
    wrefresh(wstatus);
}

void screenResize() {
    int w, h;
    getmaxyx(stdscr, h, w);
    globals::termSize.x = w;
    globals::termSize.y = h;
    werase(stdscr);
    if (wmgr != nullptr) 
        wmgr->refreshWidgets(globals::termSize.x, globals::termSize.y - 1);
}

void screenExit() {
    wmgr->freeWidgets();
    wclear(stdscr); 
    endwin();
    echo();
    nodelay(stdscr, FALSE);
    curs_set(true);
    refresh();
}
