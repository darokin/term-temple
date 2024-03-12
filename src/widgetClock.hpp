#ifndef TERMTEMPLE_WIDGETCLOCK_H
#define TERMTEMPLE_WIDGETCLOCK_H

#include "widget.hpp"

class WidgetClock : public Widget {
private:
    bool isBig {true};

public:
    WidgetClock(const std::wstring& _title, i2d _pos, i2d _size);
    explicit WidgetClock(const std::wstring& _title);
    ~WidgetClock();
    void draw() override;
    void handleKey(int _keycode) override;
};

#endif
