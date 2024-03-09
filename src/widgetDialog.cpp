#include <time.h>
#include <locale>
#include <sstream>
#include <fstream>
#include <codecvt>

#include "screen.hpp"
#include "renderer.hpp"
#include "widget.hpp"
#include "widgetTextFile.hpp"
#include "widgetDialog.hpp"
#include "cursesAnsi.hpp"
#include "utils.hpp"
#include "globals.hpp"

// = TODO : add position
WidgetDialog::WidgetDialog(std::wstring _title, const char* _ansiFilePath) : WidgetTextFile::WidgetTextFile(_title, _ansiFilePath) {
    dialog = nullptr;
    dialogTimeStart = globals::currentTimeInMs; // Utils::timeInMilliseconds();

    button = new ModuleButton(L"Close", {3, 5}, 12);
    button->setWidget(this);
    button->updatePos();
}

WidgetDialog::~WidgetDialog() {
    //if (txtFile.is_open())
    //    txtFile.close();
    if (dialog != nullptr)
        free(dialog);
}

void WidgetDialog::addDialog(std::vector<std::wstring>& _lines) {
    dialog = new ModuleDialog({1, 1}, this->size.x - 2, this->size.y - 2);
    dialog->setWidget(this);
    dialog->updatePos();
    dialog->setText(_lines);
}

void WidgetDialog::addDialog() {
    this->addDialog(this->contentLines);
    dialog->setWidget(this);
    dialog->updatePos();
}

// == TODO : avoir un timelapsedMs à jour pour chaque objet, à côté de timeStart
void WidgetDialog::handleKey(int _keycode) {
    if (dialog == nullptr)
        return;
    switch (_keycode)
    {
    case KEY_UP:
        dialog->dialogPrev();
        break;
    case KEY_DOWN:
        dialog->dialogNext();
        break;
    case KEY_PPAGE:
    case KEY_NPAGE:
        //if (this->readSeqIndex)
        break;
    default:
        break;
    }
}

void WidgetDialog::draw() {
    if (dialog == nullptr)
        return;
  
    dialog->mainDraw();

    //if (dialog->isLastSequence() && dialog->isDone()) 
    button->mainDraw();
    if (dialog->isDone())
        renderer::drawString(((int(this->timeLapsedMs / 200) % 2) ? L" " : L"▼"), {this->pos.x + this->size.x - 2, this->pos.y + this->size.y - 2});
        //mvwaddwstr(win, this->pos.y + this->size.y - 2, this->pos.x + this->size.x - 2, ((int(timeLapsedMs / 200) % 2) ? L" " : L"▼"));
}

void WidgetDialog::setPos(i2d _pos) {
    this->pos = _pos;
    for (auto& m : this->modules)
        m->updatePos();
    if (dialog != nullptr)
        dialog->updatePos();
    button->updatePos();
}
