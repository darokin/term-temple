#ifndef TERMTEMPLE_WIDGETANSI_H
#define TERMTEMPLE_WIDGETANSI_H

#include <memory>
#include "widget.hpp"
#include "moduleANSI.hpp"

class WidgetANSI : public Widget {
private:
    std::unique_ptr<ModuleANSI> ansiModule;
public:
    WidgetANSI(const std::wstring& _title, const std::string& _ansiFilePath);
    virtual ~WidgetANSI();
    virtual void draw() final;
    virtual void setPos(i2d _pos) final;
};

#endif
