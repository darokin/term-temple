#include <iostream>
#include <string>

#include "globals.hpp"
#include "screen.hpp"
#include "renderer.hpp"
#include "widgetsManager.hpp"
#include "widget.hpp"
#include "module.hpp"

extern WidgetManager* wmgr;

Widget::Widget(const std::wstring& _title, i2d _pos, i2d _size) : title { _title }, pos { _pos }, size { _size }, timeStart { globals::currentTimeInMs } {
}

Widget::Widget(const std::wstring& _title) : Widget(_title, {0, 0}, {40, 20}) {
} 

Widget::~Widget() {
    for (auto m : modules) {
        free(m);
    }
}

void Widget::setPos(i2d _pos) {
    this->pos = _pos;
    for (auto& m : this->modules)
        m->updatePos();
}

void Widget::setSize(i2d _size) {
    this->size = _size;
}

void Widget::setColorPair(uint8_t _colorPair) {
    this->colorPair = _colorPair;
}

void Widget::setBorder(bool _bBorder) {
    this->bBorder = _bBorder;
}

void Widget::setTitle(const std::wstring& _title) {
    this->title = _title;
}

void Widget::addModule(Module* _module) {
    _module->setWidget(this);
    _module->updatePos();
    this->modules.push_back(_module);
}

void Widget::draw() {
    renderer::drawString(this->title.c_str(), {this->pos.x, this->pos.y});
    renderer::drawString(L"WIDGET ONLY", {this->pos.x + 4, this->pos.y + 4});
}

void Widget::drawBorder() {
    for (int _tx = pos.x; _tx < this->pos.x + this->size.x; _tx++) {
        renderer::drawString(L"═", {_tx, this->pos.y});
        renderer::drawString(L"═", {_tx, this->pos.y + this->size.y - 1});
    }
    for (int _ty = pos.y; _ty < this->pos.y + this->size.y; _ty++) {
        renderer::drawString(L"║", {this->pos.x, _ty});
        renderer::drawString(L"║", {this->pos.x + this->size.x - 1, _ty});
    }   
    renderer::drawString(L"╔", {this->pos.x, this->pos.y});
    renderer::drawString(L"╗", {this->pos.x + this->size.x - 1, this->pos.y});
    renderer::drawString(L"╚", {this->pos.x, this->pos.y + this->size.y - 1});
    renderer::drawString(L"╝", {this->pos.x + this->size.x - 1, this->pos.y + this->size.y - 1});
}

void Widget::update() {
    // == Refresh time lapsed
    this->timeLapsedMs = globals::currentTimeInMs - this->timeStart;
    // == Set Color
    renderer::setColor(this->colorPair);
    // == Draw backgrounds 
    for (uint8_t _y = this->pos.y; _y < this->pos.y + this->size.y -1; _y++) 
        renderer::drawString(globals::longSpacesLine, {this->pos.x, _y}, this->size.x - 1);
    // == Draw borders  
    if (bBorder)
        this->drawBorder();
    // == Draw closing cross
    if (bClosingCross)
        renderer::drawString(L"[ X ]", {this->pos.x + this->size.x - 7, this->pos.y});
    // == Draw title
    if (bTitle)
        renderer::drawString((L" " + this->title + L" ").c_str(), {this->pos.x + 2, this->pos.y}); //{this->pos.x + this->titlePosX, this->pos.y});
    // == Draw widget
    draw();
    // == Update modules 
    for (auto m : modules) {  
        m->update();
    }
}

void Widget::mainHandleKey(int _keycode) {
    switch (_keycode) {
        case 'C':
        case 'c':
            this->colorPair++;
            if (this->colorPair == colorPairs::COLORPAIR_END)
                this->colorPair = colorPairs::COLORPAIR_DEFAULT + 1; 
            break;
        default:
            break;
    }
    this->handleKey(_keycode);
}

void Widget::handleKey(int _keycode) {
    (void)_keycode;
}

void Widget::handleMouseClick(i2d _pos) {
    Module* _clickedModule = nullptr;
    Module* _m;
    //void (*_clickFunc)(Module*, Widget*, i2d);

    _pos.x -= this->pos.x;
    _pos.y -= this->pos.y;

    // == Detect click on closing cross
    if (bClosingCross) {
        if (_pos.y == 0 && _pos.x >= this->size.x - 7 && _pos.x <= this->size.x - 3) {
            wmgr->removeWidget(this);
            return;
        }
    }

    // == Detect click on local modules
    i2d _mpos, _msize;
    for (int16_t i = modules.size() - 1; i >= 0; i--) {
        _m = modules[i];
        _mpos = _m->getPos();
        _msize = _m->getSize();
        if (_pos.x >= _mpos.x && _pos.x < _mpos.x + _msize.x \
        &&  _pos.y >= _mpos.y && _pos.y < _mpos.y + _msize.y) {
            _clickedModule = _m;
            break;
        }
    }
    if (_clickedModule != nullptr) {
        // == Trigger module click function
        //_clickFunc = _clickedModule->getTriggerClick();
        //_clickFunc(_clickedModule, this, _pos); // from module : (*triggerfunc)(this, this->widget, _pos);
    }
}
