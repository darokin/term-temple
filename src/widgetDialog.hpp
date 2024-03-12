#ifndef TERMTEMPLE_WIDGETDIALOG_H
#define TERMTEMPLE_WIDGETDIALOG_H

#include <memory>
#include "screen.hpp"
#include "widget.hpp"
#include "widgetTextFile.hpp"
#include "moduleDialog.hpp"
#include "moduleButton.hpp"

// TODO termSize doit être rename ET mis dans globals (qui devrait être rename (ou redécoupé) aussi)
extern i2d termSize;

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
    void draw() override;
    void handleKey(int _keycode) override;
    void setPos(i2d _pos) override;
};

#endif
