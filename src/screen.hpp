#ifndef TERMTEMPLE_SCREEN_H
#define TERMTEMPLE_SCREEN_H

#include "utils.hpp"
#include "widgetsManager.hpp"

void screenInit();
void screenLoop();
void screenExit();
void screenResize();
void initWidgets();
void drawWidgets();
bool handleKeys(int _keycode);

//void click1(Module* _module, Widget* _widget, i2d _pos);

#endif