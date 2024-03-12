#include <cmath>
#include "renderer.hpp"
#include "moduleScroller.hpp"

ModuleScroller::ModuleScroller(i2d _pos, uint16_t _nbColumns) 
    : Module(_pos, {_nbColumns, 1}), nbColumns {_nbColumns} {
    this->msPerChar = 200;
}

void ModuleScroller::setSpeed(long _ms) {
    this->msPerChar = _ms;
}

void ModuleScroller::draw() {
    renderer::setColor(this->colorPair);

    long startPosition {0};
    startPosition += std::fmod((this->timeLapsedMs / this->msPerChar), this->sText.length()); 
    const wchar_t* wstr = this->sText.c_str();

    uint16_t xdec = (this->centered ? (this->size.x - this->nbColumns) / 2 : 0);
    for (uint16_t i = 0; i < this->nbColumns; i++) {
        renderer::drawString(&wstr[(startPosition + i) % this->sText.length()], {this->pos.x + xdec + i, this->pos.y}, 1);
    }
}

void ModuleScroller::setText(const std::wstring& _line) {
    this->sText = _line;
}

