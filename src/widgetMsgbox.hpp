#ifndef TERMTEMPLE_WIDGETMSGBOX_H
#define TERMTEMPLE_WIDGETMSGBOX_H

#include "widget.hpp"
#include "moduleDialog.hpp"
#include "moduleButton.hpp"

class WidgetMsgbox : public Widget {
protected:
    std::wstring line {};
    ModuleDialog *dialog {nullptr};
    ModuleButton *button {nullptr};
public:
    WidgetMsgbox(const std::wstring& _msg, std::wstring _buttonMsg);
    explicit WidgetMsgbox(const std::wstring& _msg);
    virtual ~WidgetMsgbox();
    virtual void draw();
    virtual void handleKey(int _keycode);
    virtual void setPos(i2d _pos);
};

#endif
