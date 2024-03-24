#include <time.h>
#include <locale>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <memory>
#include "screen.hpp"
#include "renderer.hpp"
#include "widget.hpp"
#include "widgetTextFile.hpp"
#include "widgetDialog.hpp"
#include "cursesAnsi.hpp"
#include "utils.hpp"
#include "globals.hpp"

#define DIALOGBOX_WIDTH        32
#define DIALOGBOX_PADDING      2
#define DIALOGBOX_BUTTON_MARGIN 5

const std::wstring buttonMsg = L"Close";

WidgetDialog::WidgetDialog(const std::wstring& _content) : 
                            Widget::Widget(L""),
                            dialogTimeStart { globals::currentTimeInMs },
                            dialog { std::make_unique<ModuleDialog>(i2d{2, 2}, DIALOGBOX_WIDTH - 4, 0) } { // Y is deduced later by the content
    dialog->setWidget(this);
    this->setText(_content);
    this->setPos({(globals::termSize.x - size.x) / 2, (globals::termSize.y - size.y) / 2});
}

WidgetDialog* WidgetDialog::createDialogPtrWithFile(const std::string& _filePath) {
    std::wstringstream wss;
    std::wifstream wif(_filePath);
    if (!wif.is_open())
        return nullptr;
    wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    wss << wif.rdbuf();
    return new WidgetDialog(wss.str());
}

inline WidgetDialog* WidgetDialog::createDialogPtrWithString(const std::wstring& _content) {
    return new WidgetDialog(_content);
}

void WidgetDialog::setText(const std::wstring& _ws) {
    dialog->setText(_ws);
    this->updateContent();
}

void WidgetDialog::updateContent() {
    this->setSize({this->size.x, this->dialog->getLineMax() + (this->bHasButton ? DIALOGBOX_BUTTON_MARGIN : 0) + 2});

    if (!this->bHasButton)
        return;

    // == Updating button position
    i2d _buttonPos {int(this->size.x - (buttonMsg.length() + (DIALOGBOX_PADDING*2))) / 2, this->size.y - 4};

    if (button == nullptr) {
        button = std::make_unique<ModuleButton>(buttonMsg, _buttonPos);
        button->setWidget(this);
    } else {
        button->setInitialPos(_buttonPos);
    }
    button->updatePos();
}

WidgetDialog::~WidgetDialog() {
}

void WidgetDialog::handleKey(int _keycode) {
    if (dialog == nullptr)
        return;
    switch (_keycode)
    {
    case KEY_UP:
        dialog->dialogPrev();
        break;
    case KEY_SPACE:
    case KEY_DOWN:
        dialog->dialogNext();
        break;
    case KEY_PPAGE:
    case KEY_NPAGE:
        break;
    default:
        break;
    }
}

void WidgetDialog::endOpening() {
    this->bIsOpening = false;
    for (auto m : modules) {  
        m->setTimeStart(globals::currentTimeInMs);
    }
    if (button)
        button->setTimeStart(globals::currentTimeInMs);
    if (dialog)
        dialog->setTimeStart(globals::currentTimeInMs);
}

void WidgetDialog::draw() {
    if (!dialog) // TODO build on construct
        return;

    // == Update button if present
    if (button && bHasButton)
        button->update();

    // == Update dialog
    dialog->update();
    
    if (dialog->isDone())
        renderer::drawString(((int(this->timeLapsedMs / 200) % 2) ? L" " : L"▼"), {this->pos.x + this->size.x - 2, this->pos.y + this->size.y - 2});
}

void WidgetDialog::setPos(i2d _pos) {
    this->pos = _pos;
    for (auto& m : this->modules)
        m->updatePos();
    if (dialog)
        dialog->updatePos();
    if (button)
        button->updatePos();
}
