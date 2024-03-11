//#include <cstdio>   // std::freopen()
#include <iostream> // stdout
#include <signal.h>
#include <locale.h>
#include "screen.hpp"

const char* errorFilePath { "../data/logs/errors.txt" };

void endApp() {
    screenExit();
    // do other main 'free before exit' stuff
}

/*
void resizeSignal(int sig) {
	(void) sig;
    screenResize();
}
*/

// Handle exit and curses endwin()
void exitSignal(int sig) {
	(void) sig;
	endApp();
}

int main(int, char**) {
    // == Handle SIGINT / Ctrl+c properly
    signal(SIGINT, exitSignal);

    // == Resize is handgle with wgetch() and KEY_RESIZE by curses
    //signal(SIGWINCH, resizeSignal);
    // == RESIZE signal is no handled is SCREEN

    // == Force locale to handle Unicode
    setlocale(LC_ALL, "");

    // == Stream stderr to file
    std::FILE* _ferr = std::freopen(errorFilePath, "w", stderr);
    if (_ferr == nullptr) {
        std::cout << "Error can't write errors to '" << errorFilePath << "'" << std::endl;
    }

    screenInit();
    screenLoop();
    //screenExit(); // done in endApp()

    endApp();

    return 0;
}

