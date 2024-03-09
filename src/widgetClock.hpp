#ifndef TERMTEMPLE_WIDGETCLOCK_H
#define TERMTEMPLE_WIDGETCLOCK_H

#include "widget.hpp"

class WidgetClock : public Widget {
private:
    bool isBig {true};

public:
    WidgetClock(std::wstring _title, i2d _pos, i2d _size);
    WidgetClock(std::wstring _title);
    virtual ~WidgetClock();
    virtual void draw();
    virtual void handleKey(int _keycode);
};

#endif
