#include <sstream>
#include <memory>
#include "utils.hpp"
#include "globals.hpp"
#include "widget.hpp"
#include "widgetMsgbox.hpp"
#include "widgetsManager.hpp"

#define MSGBOX_WIDTH        32
#define MSGBOX_PADDING      2
static const wchar_t* msgboxTitle = L"_MSGBOX_";

extern WidgetManager* wmgr;

WidgetMsgbox::WidgetMsgbox(const std::wstring& _msg, const std::wstring& _buttonMsg) : 
                            Widget(msgboxTitle), 
                            dialog { std::make_unique<ModuleDialog>(i2d{MSGBOX_PADDING, MSGBOX_PADDING}, MSGBOX_WIDTH - (MSGBOX_PADDING * 2)) } {
    this->bTitle = false;
    this->bBorder = false;
    this->bClosingCross = false;

    // == Set text before using size (TODO setText should be done in ModuleDialog constructor)
    dialog->setCentered(true);
    dialog->setSpeed(10);
    dialog->setText(_msg);

    // == On calcule la taille après lecture du texte par le module dialog
    this->size.x = MSGBOX_WIDTH;
    this->size.y = dialog->getLineMax() + MSGBOX_PADDING + MSGBOX_PADDING + 3;
    this->pos.x = (globals::termSize.x - this->size.x) / 2;
    this->pos.y = (globals::termSize.y - this->size.y) / 2;

    dialog->setWidget(this);
    dialog->updatePos();

    button = std::make_unique<ModuleButton>(_buttonMsg, i2d{int(this->size.x - (_buttonMsg.length() + (MSGBOX_PADDING*2))) / 2, this->size.y - 4});
    button->setWidget(this);
    button->updatePos();
}

WidgetMsgbox::WidgetMsgbox(const std::wstring& _msg) : WidgetMsgbox(_msg, L"OK") {
}

WidgetMsgbox::~WidgetMsgbox() {
}

void WidgetMsgbox::setPos(i2d _pos) {
    this->pos = _pos;
    dialog->updatePos();
    button->updatePos();
}

void WidgetMsgbox::handleKey(int _keycode) {
    switch (_keycode) {
        case globals::keys::KeySpace:
        case globals::keys::KeyEnter:
        case globals::keys::KeyEnterNumPad:
            wmgr->closeWidget(this);
            break;
        default:
            break;
    }
}

void WidgetMsgbox::draw() {
    dialog->update();
    button->update();
}
