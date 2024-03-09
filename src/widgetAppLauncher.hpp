#ifndef TERMTEMPLE_WIDGETAPPLAUNCHER_H
#define TERMTEMPLE_WIDGETAPPLAUNCHER_H

#include "widget.hpp"
#include "moduleOneLiner.hpp"

class WidgetAppLauncher : public Widget {
protected:
    ModuleOneLiner* selectedModule {nullptr};
    uint16_t indSelected {0};
public:
    WidgetAppLauncher();
    virtual ~WidgetAppLauncher();
    virtual void draw();
    virtual void handleKey(int _keycode);
    //void setPos(i2d _pos);
};

#endif
