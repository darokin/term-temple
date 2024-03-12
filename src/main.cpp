#include <iostream>
#include <signal.h>
#include <locale.h>
#include "screen.hpp"

const char* errorFilePath { "../data/logs/errors.txt" };

void endApp() {
    screenExit();
}

// == Handle exit and curses endwin()
void exitSignal(int sig) {
    (void) sig;
    endApp();
}

int main(int, char**) {
    // == Handle SIGINT / Ctrl+c properly
    signal(SIGINT, exitSignal);

    // == Resize is handled with wgetch() and KEY_RESIZE by curses
    //signal(SIGWINCH, resizeSignal);
    // == RESIZE signal is now handled is SCREEN

    // == Force locale to handle Unicode
    setlocale(LC_ALL, "");

    // == Stream stderr to file
    std::FILE* _ferr = std::freopen(errorFilePath, "w", stderr);
    if (_ferr == nullptr) {
        std::cout << "Error can't write errors to '" << errorFilePath << "'" << std::endl;
    }

    // == Start screen and do main loop
    screenInit();
    screenLoop();

    // == Screen Exit and other stuff before exit
    endApp();

    return 0;
}

