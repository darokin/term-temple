#ifndef TERMTEMPLE_WIDGETAPPLAUNCHER_H
#define TERMTEMPLE_WIDGETAPPLAUNCHER_H

#include <functional>
#include <map>
#include "widget.hpp"
#include "moduleOneLiner.hpp"
#include "widgetsManager.hpp"

/*
typedef  void (WidgetManager::*wMgrMethod) (void);

struct stAppElement {
    //class WidgetManager;

    std::function<wMgrMethod> launchFunc;
    ModuleOneLiner* module;
    stAppElement(std::function<wMgrMethod> _launchFunc) : launchFunc { _launchFunc } { }; 
};
*/

class WidgetAppLauncher : public Widget {
protected:
    ModuleOneLiner* selectedModule {nullptr};
    uint16_t indSelected {0};
    //std::map<const std::wstring, stAppElement>::iterator selectedModuleIterator {};
public:
    WidgetAppLauncher();
    ~WidgetAppLauncher();
    virtual void draw() final;
    virtual void handleKey(int _keycode) final;
};

#endif
