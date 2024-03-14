#ifndef TERMTEMPLE_WIDGETDIALOG_H
#define TERMTEMPLE_WIDGETDIALOG_H

#include <memory>
#include "screen.hpp"
#include "widget.hpp"
#include "widgetTextFile.hpp"
#include "moduleDialog.hpp"
#include "moduleButton.hpp"

class WidgetDialog : public WidgetTextFile {
private:
    long long dialogTimeStart;
    std::unique_ptr<ModuleDialog> dialog {};
    std::unique_ptr<ModuleButton> button {};
public:
    WidgetDialog(const std::wstring& _title, const char* _ansiFilePath);
    virtual ~WidgetDialog();
    void addDialog(std::vector<std::wstring>& _lines);
    void addDialog();
    virtual void draw() final;
    virtual void handleKey(int _keycode) final;
    virtual void setPos(i2d _pos) final;
};

#endif
