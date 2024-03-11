#include "globals.hpp"
#include "utils.hpp"
#include "renderer.hpp"
#include "module.hpp"
#include "widget.hpp"

Module::Module(i2d _initialPos, i2d _size): initialPos {_initialPos}, pos {_initialPos}, size {_size} {
    this->timeStart = globals::currentTimeInMs; 
    this->widget = nullptr;
    this->clickFunc = nullptr;
}

Module::~Module() {
}

void Module::updatePos() {
    if (this->widget == nullptr) { // for special modules like the background that are not on a widget
        this->pos.x = this->initialPos.x;
        this->pos.y = this->initialPos.y;
        return;
    }
    i2d _widgetPos = this->widget->getPos();
    this->pos.x = this->initialPos.x + _widgetPos.x;
    this->pos.y = this->initialPos.y + _widgetPos.y;
}

void Module::mainDraw() {
    this->timeLapsedMs = globals::currentTimeInMs - this->timeStart;

    // == Set Color
    renderer::setColor(this->colorPair);
    // == Set Reverse if needed
    if (this->colorReversed)
        renderer::setColorReverse(true);
    // == Draw backgrounds 
    for (uint8_t _y = this->pos.y; _y < this->pos.y + this->size.y -1; _y++) 
        renderer::drawString(globals::longSpacesLine, {this->pos.x, _y}, this->size.x - 1);

    draw();

    // == Remove reversed if used
    if (this->colorReversed)
        renderer::setColorReverse(false);
}

void Module::draw() {
}

void Module::setTriggerClick(triggerfunc _clickFunc) {
    this->clickFunc = _clickFunc; 
    //(*triggerfunc)(this, this->widget, _pos);
};
