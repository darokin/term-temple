#include "moduleButton.hpp"
#include "renderer.hpp"

#define BUTTON_DEFAULT_HEIGHT   3

ModuleButton::ModuleButton(const std::wstring& _wstr, i2d _pos, uint16_t _nbColumns) 
 : Module(_pos, {_nbColumns, BUTTON_DEFAULT_HEIGHT}), sText {_wstr}, nbColumns {_nbColumns} {
    this->msPerChar = 200;
}

ModuleButton::ModuleButton(const std::wstring& _wstr, i2d _pos): ModuleButton(_wstr, _pos, _wstr.length() + 4) {
}

void ModuleButton::setText(const std::wstring& _line) {
    this->sText = _line;
}

void ModuleButton::draw() {
    //renderer::drawString(this->sText.c_str(), {this->pos.x + int((this->size.x - this->sText.length() - 4) / 2), this->pos.y + 1}, (this->nbColumns));
    renderer::drawString(this->sText.c_str(), {this->pos.x + 2, this->pos.y + 1});//, {this-} {this->pos.x + int((this->size.x - this->sText.length() - 4) / 2), this->pos.y + 1}, (this->nbColumns));
    for (int _tx = pos.x; _tx < this->pos.x + this->size.x; _tx++) {
        renderer::drawString(L"─", {_tx, this->pos.y});
        renderer::drawString(L"─", {_tx, this->pos.y + this->size.y - 1});
    }
    for (int _ty = pos.y; _ty < this->pos.y + this->size.y; _ty++) {
        renderer::drawString(L"│", {this->pos.x, _ty});
        renderer::drawString(L"│", {this->pos.x + this->size.x - 1, _ty});
    }   
    renderer::drawString(L"┌", {this->pos.x, this->pos.y});
    renderer::drawString(L"┐", {this->pos.x + this->size.x - 1, this->pos.y});
    renderer::drawString(L"└", {this->pos.x, this->pos.y + this->size.y - 1});
    renderer::drawString(L"┘", {this->pos.x + this->size.x - 1, this->pos.y + this->size.y - 1});
}



