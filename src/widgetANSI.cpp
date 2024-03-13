#include "screen.hpp" // getColor and colorPairs
#include "widget.hpp"
#include "widgetANSI.hpp"
#include "cursesAnsi.hpp"

/*
// TODO : Mettre un moduleANSI inside et avoir un draw d'une ligne
WidgetANSI::WidgetANSI(const std::wstring& _title, i2d _pos, i2d _size) : Widget::Widget(_title, _pos, _size) {
}

WidgetANSI::WidgetANSI(const std::wstring& _title) : Widget::Widget(_title) {
}
*/

WidgetANSI::WidgetANSI(const std::wstring& _title, const char* _ansiFilePath) : Widget::Widget(_title) {
    // , ansiModule { _ansiFilePath }
    ansiModule = std::make_unique<ModuleANSI>(_ansiFilePath);
    i2d ansiSize = ansiModule->getSize();
    ansiSize.x += 2;
    ansiSize.y += 2;
    this->setSize(ansiSize);
    ansiModule->setWidget(this);
    ansiModule->updatePos();
    //this->bBorder = false;
    this->setColorPair(getColor(colorPairs::WHITE_ON_BLACK));
    //ansiFile.open(_ansiFilePath);
    //if (!ansiFile.is_open())
    //    std::cout << "Unable to open ANSI file '" << _ansiFilePath << "'" << std::endl; 
}

WidgetANSI::~WidgetANSI() {
    // file is auto closed
}

void WidgetANSI::draw() {
    ansiModule->draw();
    /*
    if (!ansiFile.is_open())
        return;
    std::string _line;
    int y { 0 };
    this->ansiFile.clear();
    this->ansiFile.seekg(0);
    while (getline(this->ansiFile, _line) && y < this->size.y) {
        if (!cursesANSI::mvwaddnANSIstr(this->pos.y + y, this->pos.x + 1, _line))
            break;
        y++;
    }
    */
}

void WidgetANSI::setPos(i2d _pos) {
    this->pos = _pos;
    ansiModule->updatePos();
}