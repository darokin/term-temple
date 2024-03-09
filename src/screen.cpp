#include <iostream>
#include <sstream>
#include <vector>

#ifdef _WIN32
	#include <windows.h>
	#define NCURSES_WIDECHAR 1
    #include <ncursesw/ncurses.h>
	#define msleep(x) Sleep((x))
#else // __unix__
	#include <unistd.h> // usleep()
    #include <ncursesw/curses.h>
	#include <ncursesw/menu.h>
	#define msleep(x) usleep((x) * 100)   // todo faire inline à la place ?
#endif

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

i2d termSize;
WidgetManager* wmgr;
WINDOW *wstatus;

// TODO remove all color dec if unused... getColor() is useless
#define COLOR_DEC_CONST			32
#define COLOR_BRIGHT_BLUE		COLOR_DEC_CONST + 9
#define COLOR_BRIGHT_MAGENTA	COLOR_DEC_CONST + 13
#define COLOR_BRIGHT_YELLOW		COLOR_DEC_CONST + 14
#define COLOR_BRIGHT_WHITE		COLOR_DEC_CONST + 15

void initWidgets() {
	
	wmgr = WidgetManager::getInstance();
	wmgr->setBackground("../data/ans/dar_0_home.ans");

/*	
	// ================================================
	// == HEADER ROW 1
	//WidgetTextFile* wHeader = new WidgetTextFile(L"HEADER ♞", "widget.cpp");
	//WidgetANSI* wHeader = new WidgetANSI(L"HEADER ♞", "/home/darokin/ascii/darokin_cmyk-skull_mist1023_01.ans");//dar_0_2024.ans");
	//dar_0_home.ans
	
	WidgetANSI* wHeader = new WidgetANSI(L"HEADER ♞", "./dar_0_home.ans");
	wHeader->setPos({0, 0});
	wHeader->setSize({termSize.x, termSize.y - 1});
	//wHeader->setColorPair(getColor(colorPairs::BLUE_ON_WHITE));

	std::vector<std::wstring> vs {\
		L"░▒▒▓▓██████▓▓▒▒░", \
		L"░▒▒▓▓████████▓▓▒▒░", \
		L"░░▒▒▒▓▓▓█████████▓▓▓▒▒▒░░", \
		L"░░▒▒▒▓▓▓▓███████████▓▓▓▓▒▒▒░░", \
		L"░░▒▒▒▓▓▓█████████▓▓▓▒▒▒░░", \
		L"░▒▒▓▓████████▓▓▒▒░", \
		L"░▒▒▓▓██████▓▓▒▒░", \
		L"░▒▒▓▓████████▓▓▒▒░", \
		L"░▒▒▓▓▓███████▓▓▓▒▒░", \
		L"░▒▒▒▓▓▓███████▓▓▓▒▒▒░", \
		L"░░▒▒▓▓▓██████▓▓▓▒▒░░", \
		L"░░▒▒▓▓██████▓▓▒▒░░" \
	};

	std::vector<std::wstring> vs2 {L"Why", L"is", L"it", L"like", L"this", L",", L"anybody?"};
	
	ModuleScroller* tmpScroller;

	for (int y = 0; y < termSize.y - 1; y++) {
		tmpScroller = new ModuleScroller({12, y}, vs.at(y % vs.size()).length());
		tmpScroller->setText(vs.at(y % vs.size()));
		tmpScroller->setColorPair(getColor(colorPairs::YELLOW_ON_BLUE));
		tmpScroller->setSpeed(100 - (y * 4));
		tmpScroller->setSize({32, 1});
		tmpScroller->setCentered(true);
		tmpScroller->setWidget(wHeader);
    	tmpScroller->updatePos();
		wHeader->addModule(tmpScroller);
	}
	
	ModuleOneLiner* selector = new ModuleOneLiner(L"TEST one line...", {1, 1});
    selector->setWidget(wHeader);
    selector->updatePos();
	wHeader->addModule(selector);
*/	

	/*
	WidgetDialog* wMsg = new WidgetDialog(L"INFO BOX ▼ ", "text.txt");
	wMsg->setPos({4, 18});
	wMsg->setSize({42, 10});
	wMsg->addDialog();
	wMsg->setColorPair(getColor(colorPairs::YELLOW_ON_BLACK));
	wmgr->addWidget(wMsg);
	*/

/*	
	WidgetMsgbox* wMsg = new WidgetMsgbox(L" ▼ ALERT ▼ \n\nThis is an emergency broadcast.\nComing directly from the emergency broadcast sytem live on WWWap.");
	//wMsg->setPos({4, 18});
	//wMsg->setSize({42, 10});
	wMsg->setColorPair(getColor(colorPairs::YELLOW_ON_BLACK));
	
	WidgetClock* wClock = new WidgetClock(L"CLOCK", {10, 8}, {48, 12});
	wClock->setColorPair(getColor(colorPairs::BLUE_ON_YELLOW));

	WidgetAppLauncher* wAppLauncher = new WidgetAppLauncher();


	wmgr->addWidget(wHeader);
	wmgr->addWidget(wClock);
	wmgr->addWidget(wMsg);
	wmgr->addWidget(wAppLauncher);
*/	
}

/*
void click1(Module* _module, Widget* _widget, i2d _pos) {
	// TODO : size parameter to alert (small / medium / big)
	wmgr->alert(L"CLICKED");
}
*/

void drawStatusBar() {
	werase(wstatus);
	wbkgd(wstatus, COLOR_PAIR(getColor(colorPairs::BLUE_ON_WHITE)) | A_BOLD | A_REVERSE);
	wmove(wstatus, 0, 0);
	wprintw(wstatus, "%ls", globals::getStatusText());
	wrefresh(wstatus);
}

void screenInit() {
	// == Init screen
	initscr();
	wclear(stdscr);
	cbreak(); 				// one char at a time (do not buffer till enter)
	curs_set(FALSE);		// hide cursor
	noecho(); 				// suppress echoing of keys input
	keypad(stdscr, TRUE); 	// allow getch() to listen to arrows and other special keys
	nodelay(stdscr, TRUE);	// non blocking getch()
    nonl();                 // no new line / catch enter key

	// == Init color pairs
	//if (has_colors()) {  
		use_default_colors();
		start_color();
	//};
	use_default_colors();
	if (can_change_color() && COLORS >= 16) {
        init_color(COLOR_BRIGHT_WHITE, 1000, 1000, 1000);
		init_color(COLOR_BRIGHT_MAGENTA, 1000, 0, 0);
		init_color(COLOR_BRIGHT_BLUE, 0, 0, 1000);
		init_color(COLOR_BRIGHT_YELLOW, 1000, 1000, 0);

		init_pair(getColor(colorPairs::BLACK_ON_WHITE), COLOR_BLACK, COLOR_BRIGHT_WHITE);
		init_pair(getColor(colorPairs::YELLOW_ON_WHITE), COLOR_YELLOW, COLOR_BRIGHT_WHITE);
		init_pair(getColor(colorPairs::PINK_ON_WHITE), COLOR_MAGENTA, COLOR_BRIGHT_WHITE);	
		init_pair(getColor(colorPairs::BLUE_ON_WHITE), COLOR_BLUE, COLOR_BRIGHT_WHITE);
		init_pair(getColor(colorPairs::BLUE_ON_YELLOW), COLOR_BLUE, COLOR_BRIGHT_YELLOW);
		init_pair(getColor(colorPairs::BLACK_ON_YELLOW), COLOR_BLACK, COLOR_BRIGHT_YELLOW);
		init_pair(getColor(colorPairs::YELLOW_ON_BLUE), COLOR_YELLOW, COLOR_BRIGHT_BLUE);
		init_pair(getColor(colorPairs::BLACK_ON_BLUE), COLOR_BLACK, COLOR_BRIGHT_BLUE);
		init_pair(getColor(colorPairs::BLACK_ON_PINK), COLOR_BLACK, COLOR_BRIGHT_MAGENTA);	
	} else {
		init_pair(getColor(colorPairs::BLACK_ON_WHITE), COLOR_BLACK, COLOR_WHITE);
		init_pair(getColor(colorPairs::YELLOW_ON_WHITE), COLOR_YELLOW, COLOR_WHITE);
		init_pair(getColor(colorPairs::PINK_ON_WHITE), COLOR_MAGENTA, COLOR_WHITE);	
		init_pair(getColor(colorPairs::BLUE_ON_WHITE), COLOR_BLUE, COLOR_WHITE);
		init_pair(getColor(colorPairs::BLUE_ON_YELLOW), COLOR_BLUE, COLOR_YELLOW);
		init_pair(getColor(colorPairs::BLACK_ON_YELLOW), COLOR_BLACK, COLOR_YELLOW);
		init_pair(getColor(colorPairs::YELLOW_ON_BLUE), COLOR_YELLOW, COLOR_BLUE);
		init_pair(getColor(colorPairs::BLACK_ON_BLUE), COLOR_BLACK, COLOR_BLUE);
		init_pair(getColor(colorPairs::BLACK_ON_PINK), COLOR_BLACK, COLOR_MAGENTA);	
	}
	init_pair(colorPairs::DEFAULT, -1, -1);
	init_pair(getColor(colorPairs::WHITE_ON_BLACK), COLOR_WHITE, COLOR_BLACK);
	init_pair(getColor(colorPairs::YELLOW_ON_BLACK), COLOR_YELLOW, COLOR_BLACK);
	init_pair(getColor(colorPairs::PINK_ON_BLACK), COLOR_MAGENTA, COLOR_BLACK);	
	init_pair(getColor(colorPairs::BLUE_ON_BLACK), COLOR_BLUE, COLOR_BLACK);

	// == Mouse events
	mousemask(ALL_MOUSE_EVENTS, NULL);

	// == Init term size by faking a resize
	screenResize();

	// == Create main wmaindow and initialize default style
	//wMain = newwmain(viewSize.y, viewSize.x, 0, 0);
	wstatus = newwin(1, termSize.x, termSize.y - 1, 0);
	
	// == Init global time (do it once so widget constructors get their timeStart)
	globals::tick();

	// == Init widgets
    initWidgets();
}

void screenLoop() {
    int keycode;
	bool bQuit = false;
	MEVENT event;

    do {
		msleep(80);
        
		// ============================================
		globals::tick();
		//werase(stdscr);
		wmgr->draw();
		drawStatusBar();
		wrefresh(stdscr);
		// ============================================

		// == KEY LISTENING
		keycode = wgetch(stdscr);
		if (keycode == -1)
			continue;

		// == TODO : Faire marcher PC speaker 
		if (keycode) {
			beep();
		}
		switch (keycode) {
			case KEY_MOUSE:
				if(getmouse(&event) != OK)
					break;
				if(event.bstate & BUTTON1_PRESSED) {	
					//swprintf(statusText, STATUS_TEXT_MAXLENGTH, L"Mouse 1 [%d, %d]", event.x, event.y);
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
				wmgr->refreshWidgets(termSize.x, termSize.y);
				break;
			case KEY_F(2):
				wmgr->toggleAppLauncher();
				//wmgr->alert(L"F2 KEY PRESSED");
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
				//swprintf(statusText, STATUS_TEXT_MAXLENGTH, L"Key '%d' '0x%x' [%c]", keycode, keycode, (char)keycode);
				wrefresh(wstatus);
		}
	} while(!bQuit);
}

void statusRefresh() {
	wrefresh(wstatus);
}

void screenResize() {
	int w, h;
	getmaxyx(stdscr, h, w);
	termSize.x = w;
	termSize.y = h;
	//swprintf(statusText, STATUS_TEXT_MAXLENGTH, L" \u265E TermTemple ♞ v0.1 w = [%d, %d]", termSize.x, termSize.y); 
	werase(stdscr);
	if (wmgr != nullptr) 
		wmgr->refreshWidgets(termSize.x, termSize.y - 1);
}

void screenExit() {
	wclear(stdscr); 
	endwin();
	echo();
	nodelay(stdscr, FALSE);
	curs_set(true);
	refresh();
}
