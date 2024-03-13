#include "screen.hpp" // getColor and colorPairs
#include "widget.hpp"
#include "widgetANSI.hpp"
#include "cursesAnsi.hpp"

WidgetANSI::WidgetANSI(const std::wstring& _title, const char* _ansiFilePath) : Widget::Widget(_title) {
    // == Init ANSI module and set position
    ansiModule = std::make_unique<ModuleANSI>(_ansiFilePath);
    i2d ansiSize = ansiModule->getSize();
    ansiSize.x += 2;
    ansiSize.y += 2;
    // == Set widget size according to module ansi content (size), so before updatePos()
    this->setSize(ansiSize);
    ansiModule->setWidget(this);
    ansiModule->updatePos();
    // == Widget Color
    this->setColorPair(getColor(colorPairs::WHITE_ON_BLACK));
}

WidgetANSI::~WidgetANSI() {
    // file is auto closed
}

void WidgetANSI::draw() {
    ansiModule->draw();
}

void WidgetANSI::setPos(i2d _pos) {
    this->pos = _pos;
    ansiModule->updatePos();
}