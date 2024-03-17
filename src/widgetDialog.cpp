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

const std::wstring buttonMsg = L"Close";

// == TODO : add position
WidgetDialog::WidgetDialog(const std::wstring& _title, const std::string& _ansiFilePath) : WidgetTextFile::WidgetTextFile(_title, _ansiFilePath) {
    dialogTimeStart = globals::currentTimeInMs; // Utils::timeInMilliseconds(); (TODO)

    // == On calcule la taille après lecture du texte par le module dialog
    // TODO toute la classe à revoir
    if (_ansiFilePath != "") {
        addDialog(); // TODO : Do static initers that call a simplier constructor, do the dirty and return a new dialog (?)
    }

    button = std::make_unique<ModuleButton>(buttonMsg, i2d{int(this->size.x - (buttonMsg.length() + (DIALOGBOX_PADDING*2))) / 2, this->size.y - 4});
    button->setWidget(this);
    button->updatePos();
}

WidgetDialog::~WidgetDialog() {
}

// TODO : c'est pas clair du tout entre loader le dialog depuis un vecteur de lignes construit à l'extérieur
// ou le addDialog() qui charge le dialog depuis le fichier chargé (devrait être fait dans le constructeur)
void WidgetDialog::addDialog(std::vector<std::wstring>& _lines) {
    dialog = std::make_unique<ModuleDialog>(i2d{2, 2}, DIALOGBOX_WIDTH - 4, this->size.y - 4);
    dialog->setText(_lines);

    this->size.x = DIALOGBOX_WIDTH;
    this->size.y = dialog->getLineMax() + DIALOGBOX_PADDING + DIALOGBOX_PADDING + 3;
    this->pos.x = (globals::termSize.x - this->size.x) / 2;
    this->pos.y = (globals::termSize.y - this->size.y) / 2;

    dialog->setWidget(this);
    dialog->updatePos();
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
    if (!dialog) // TODO build on construct
        return;
  
    dialog->update();

    button->update();
    if (dialog->isDone())
        renderer::drawString(((int(this->timeLapsedMs / 200) % 2) ? L" " : L"▼"), {this->pos.x + this->size.x - 2, this->pos.y + this->size.y - 2});
}

void WidgetDialog::setPos(i2d _pos) {
    this->pos = _pos;
    for (auto& m : this->modules)
        m->updatePos();
    if (dialog) // != nullptr)
        dialog->updatePos();
    button->updatePos();
}
