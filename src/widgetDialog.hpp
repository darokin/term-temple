#ifndef TERMTEMPLE_WIDGETDIALOG_H
#define TERMTEMPLE_WIDGETDIALOG_H

#include <memory>
#include "screen.hpp"
#include "widget.hpp"
#include "widgetTextFile.hpp"
#include "moduleDialog.hpp"
#include "moduleButton.hpp"

class WidgetDialog : public Widget {
private:
    long long                       dialogTimeStart;
    std::unique_ptr<ModuleDialog>   dialog;
    std::unique_ptr<ModuleButton>   button {nullptr};
    bool                            bHasButton {true};

    void updateContent(); // to handle resize based on content after setText (button location etc.)
    WidgetDialog(const std::wstring& _content);
public:
    static WidgetDialog* createDialogPtrWithFile(const std::string& _filePath);
    static WidgetDialog* createDialogPtrWithString(const std::wstring& _content);

    virtual ~WidgetDialog();
    void    setText(const std::wstring& _ws);
    void    setText(std::vector<std::wstring>& _lines);
    void    draw()                  final;
    void    endOpening()            final;
    void    handleKey(int _keycode) final;
    void    setPos(i2d _pos)        final;
    void    setSpeed(float _speed) { if (this->dialog) this->dialog->setSpeed(_speed); };
    void    setCentered(bool _centered) { if (this->dialog) this->dialog->setCentered(_centered); };
    void    setHasButton(bool _hasButton) { this->bHasButton = _hasButton; };
};

#endif
