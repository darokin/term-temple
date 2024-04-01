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

void Widget::endOpening() {
}

void Widget::startClosing() {
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

void Widget::close() {
    if (this->state >= widgetState::STATE_CLOSING)
        return;
    this->timeClosingStart = globals::currentTimeInMs;
    this->state = widgetState::STATE_CLOSING;
}

void Widget::draw() {
    renderer::drawString(this->title.c_str(), {this->pos.x, this->pos.y});
    renderer::drawString(L"WIDGET ONLY", {this->pos.x + 4, this->pos.y + 4});
}

void Widget::drawBorder(i2d _framePos, i2d _frameSize) {
    for (int _tx = pos.x; _tx < _framePos.x + _frameSize.x; _tx++) {
        renderer::drawString(L"═", {_tx, _framePos.y});
        renderer::drawString(L"═", {_tx, _framePos.y + _frameSize.y - 1});
    }
    for (int _ty = pos.y; _ty < _framePos.y + _frameSize.y; _ty++) {
        renderer::drawString(L"║", {_framePos.x, _ty});
        renderer::drawString(L"║", {_framePos.x + _frameSize.x - 1, _ty});
    }   
    renderer::drawString(L"╔", {_framePos.x, _framePos.y});
    renderer::drawString(L"╗", {_framePos.x + _frameSize.x - 1, _framePos.y});
    renderer::drawString(L"╚", {_framePos.x, _framePos.y + _frameSize.y - 1});
    renderer::drawString(L"╝", {_framePos.x + _frameSize.x - 1, _framePos.y + _frameSize.y - 1});
}

void Widget::drawFrame() {
    i2d _framePos = this->pos;
    i2d _frameSize = this->size;

    // == Opening calculation for an opening effect
    if (this->state == widgetState::STATE_OPENING) {// this->timeOpeningMs > 0 && this->timeLapsedMs < this->timeOpeningMs) {
        double _ratioOpening = Utils::easeOutCubic(((double)this->timeLapsedMs / (double)this->timeOpeningMs));
        v2d<double> _minSize { (double)this->size.x * 0.4, (double)this->size.y * 0.4 };
        _frameSize.x = (_minSize.x) + ((this->size.x - _minSize.x) * _ratioOpening);
        _frameSize.y = (_minSize.y) + ((this->size.y - _minSize.y) * _ratioOpening);
        //_framePos.x += ((this->size.x - _frameSize.x) / 2);
    } else if (this->state == widgetState::STATE_CLOSING) {
        double _ratioClosing = Utils::easeInCubic(((double)(globals::currentTimeInMs - this->timeClosingStart) / (double)this->timeOpeningMs));
        v2d<double> _minSize { 4, 4 };//(double)this->size.x * 0.15, (double)this->size.y * 0.15 };
        _frameSize.x = (this->size.x) - ((this->size.x - _minSize.x) * _ratioClosing);
        _frameSize.y = (this->size.y) - ((this->size.y - _minSize.y) * _ratioClosing);
    }
    // == Set Color
    renderer::setColor(this->colorPair);
    // == Draw backgrounds
    for (uint8_t _y = _framePos.y; _y < _framePos.y + _frameSize.y -1; _y++)
        renderer::drawString(globals::longSpacesLine, {_framePos.x, _y}, _frameSize.x - 1);
    // == Draw borders  
    if (bBorder)
        this->drawBorder(_framePos, _frameSize);
    // == Draw closing cross
    if (bClosingCross)
        renderer::drawString(L"[ X ]", {_framePos.x + _frameSize.x - 7, _framePos.y});
    // == Draw title
    if (bTitle && !this->title.empty())
        renderer::drawString((L" " + this->title + L" ").c_str(), {_framePos.x + 2, _framePos.y}); //{this->pos.x + this->titlePosX, this->pos.y});
}

void Widget::updateState() {
    // == Check the 'opening' state ending
    if (this->state == widgetState::STATE_OPENING && this->timeLapsedMs > this->timeOpeningMs) {
        this->state = widgetState::STATE_NORMAL;
        for (auto m : modules)
            m->setTimeStart(globals::currentTimeInMs);
        endOpening();
    // == Chech the 'closing' state ending
    } else if (this->state == widgetState::STATE_CLOSING && globals::currentTimeInMs - this->timeClosingStart > this->timeClosingMs) {
        this->state = widgetState::STATE_KILLED;
        for (auto m : modules)
            free(m);
        modules.clear();
    }
}

void Widget::update() {
    // == Refresh time lapsed
    this->timeLapsedMs = globals::currentTimeInMs - this->timeStart;
    // == Update state (opening / closing animation)
    updateState();
    // == Draw frame (bg, border, title and closing cross)
    drawFrame();
    // == If the widget is opening, only show frame opening
    if (this->isOpening())
        return;
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
            wmgr->closeWidget(this);
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
