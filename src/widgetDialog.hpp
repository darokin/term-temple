#ifndef TERMTEMPLE_WIDGETDIALOG_H
#define TERMTEMPLE_WIDGETDIALOG_H

#include "screen.hpp"
#include "widget.hpp"
#include "widgetTextFile.hpp"
#include "moduleDialog.hpp"
#include "moduleButton.hpp"

extern i2d termSize;

class WidgetDialog : public WidgetTextFile {
private:
    long long dialogTimeStart;
    ModuleDialog* dialog;
    ModuleButton* button;
public:
    WidgetDialog(std::wstring _title, const char* _ansiFilePath);
    virtual ~WidgetDialog();
    virtual void draw();
    virtual void handleKey(int _keycode);
    void addDialog(std::vector<std::wstring>& _lines);
    void addDialog();
    virtual void setPos(i2d _pos);
};

#endif
