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
    long long dialogTimeStart {};
    std::unique_ptr<ModuleDialog> dialog {nullptr};
    std::unique_ptr<ModuleButton> button {nullptr};
public:
    WidgetDialog(const std::wstring& _title, const std::string& _ansiFilePath);
    virtual ~WidgetDialog();
    void addDialog(std::vector<std::wstring>& _lines);
    void addDialog();
    virtual void draw() final;
    virtual void handleKey(int _keycode) final;
    virtual void setPos(i2d _pos) final;
    void setCentered(bool _centered) { if (this->dialog) dialog->setCentered(_centered); };
};

#endif
