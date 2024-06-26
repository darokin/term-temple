#include <string.h>
#include "moduleOneLiner.hpp"
#include "renderer.hpp"
#include "globals.hpp"
#include "utils.hpp"

ModuleOneLiner::ModuleOneLiner(const std::wstring& _line, i2d _pos)
    : Module(_pos, {(int)_line.length(), 1}) {
    this->setText(_line);
}

void ModuleOneLiner::draw() {
    uint16_t nbCharsToShow {0};
    nbCharsToShow = Utils::max(this->timeLapsedMs / this->msPerChar, this->nbChars);
    nbCharsToShow = Utils::max(nbCharsToShow, this->size.x);
    renderer::drawString(this->line.c_str(), {this->pos.x, this->pos.y}, nbCharsToShow);
    this->bIsDone = (nbCharsToShow >= this->nbChars);
}

void ModuleOneLiner::setText(const std::wstring& _line) {
    this->line = _line;
    this->nbChars = _line.length();
    this->size.x = this->nbChars;
}
