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
    ~WidgetAppLauncher();
    void draw() override;
    void handleKey(int _keycode) override;
};

#endif
