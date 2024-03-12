#include "screen.hpp" // getColor and colorPairs
#include "widget.hpp"
#include "widgetANSI.hpp"
#include "cursesAnsi.hpp"

// TODO : Mettre un moduleANSI inside et avoir un draw d'une ligne
WidgetANSI::WidgetANSI(const std::wstring& _title, i2d _pos, i2d _size) : Widget::Widget(_title, _pos, _size) {
}

WidgetANSI::WidgetANSI(const std::wstring& _title) : Widget::Widget(_title) {
}

WidgetANSI::WidgetANSI(const std::wstring& _title, const char* _ansiFilePath) : Widget::Widget(_title) {
    //this->bBorder = false;
    this->setColorPair(getColor(colorPairs::WHITE_ON_BLACK));
    ansiFile.open(_ansiFilePath);
    if (!ansiFile.is_open())
        std::cout << "Unable to open ANSI file '" << _ansiFilePath << "'" << std::endl; 
}

WidgetANSI::~WidgetANSI() {
    // file is auto closed
}

void WidgetANSI::draw() {
    if (!ansiFile.is_open())
        return;
    std::string _line;
    int y{ 0 };
    this->ansiFile.clear();
    this->ansiFile.seekg(0);
    while (getline(this->ansiFile, _line) && y < this->size.y) {
        if (!cursesANSI::mvwaddnANSIstr(this->pos.y + y, this->pos.x + 1, _line))
            break;
        y++;
    }
}
