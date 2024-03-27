#include <sstream>
#include <fstream>
#include <locale>       // for wstring to string conversion
#include <codecvt>      // for wstring to string conversion
#include <algorithm>    // to use std::replace_if()
#include "globals.hpp"
#include "screen.hpp"
#include "renderer.hpp"
#include "widget.hpp"
#include "widgetTextFile.hpp"
#include "cursesAnsi.hpp"

WidgetTextFile::WidgetTextFile(const std::wstring& _title, const std::string& _filePath) : Widget::Widget(_title) {
    this->bResizable = true;

    // == File reading and conversion
    std::wstringstream wss;
    std::wifstream wif(_filePath);
    if (!wif.is_open()) {
        wss << "Unable to open TXT file '" << _filePath.c_str() << "'" << std::endl;
        this->contentLines.push_back(wss.str());
        return;
    }
    wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    wss << wif.rdbuf();

    // == Store lines from wifstream to vector of lines 'contentLines'
    // == Remove the TABS 
    // == Calculate the maximum width 'maxWidth'  
    std::wstring _line;
    while (getline(wss, _line)) {
        // == Better than nothing changing tabulations for spaces, tabulations mess string length count and nb chars/columns
        std::replace_if(_line.begin(), _line.end(), [] (const wchar_t& wc) { return (wc == '\t') ;}, ' ');
        this->contentLines.push_back(_line);
        // == Store the maximum size in chars
        if (_line.size() > this->maxWidth)
            this->maxWidth = _line.size();
    }
    this->maxHeight = this->contentLines.size();
}

WidgetTextFile::~WidgetTextFile() {
    // auto closing wifstream
}

void WidgetTextFile::handleKey(int _keycode) {
    switch (_keycode) {
        // == SCROLL VERTICAL Y
        case globals::keys::KeyUp:
            if (this->scrollY > 0)
                this->scrollY--;
            break;
        case globals::keys::KeyDown:
            if (this->scrollY < (int)this->maxHeight - this->size.y + 2)
                this->scrollY++;
            break;
        case globals::keys::KeyPageUp:
            if (this->scrollY - this->size.y - 2 < 0)
                this->scrollY = 0;
            else
                this->scrollY -= this->size.y - 2;
            break;
        case globals::keys::KeyPageDown:
            this->scrollY += this->size.y - 2;
            if (this->scrollY > (int)this->maxHeight - this->size.y + 2)
                this->scrollY = this->maxHeight - this->size.y + 2;
            break;
        // == SCROLL HORIZONTAL X
        case globals::keys::KeyLeft:
            if (this->scrollX > 0)
                this->scrollX--;
            break;
        case globals::keys::KeyRight:
            if (this->scrollX < this->maxWidth - this->size.x + 2)
                this->scrollX++;
            break;
        case globals::keys::KeyShiftLeft:
            if (this->scrollX - this->size.x - 2 < 0)
                this->scrollX = 0;
            else
                this->scrollX -= this->size.x - 2;
            break;
        case globals::keys::KeyShiftRight:
            this->scrollX += this->size.x - 2;
            if (this->scrollX > this->maxWidth - this->size.x + 2)
                this->scrollX = this->maxWidth - this->size.x + 2;
            break;
        default:
            break;
    }
}

void WidgetTextFile::draw() {
    std::wstring* content;
    std::wstring tmpWString;
    float _nbLines = static_cast<float>(this->maxHeight);
    float _nbLinesDisplayable = static_cast<float>(this->size.y - 2);
    float _nbCols = static_cast<float>(this->maxWidth);
    float _nbColsDisplayable = static_cast<float>(this->size.x - 2);

    // == Draw file content
    for (uint16_t nLine = this->scrollY; nLine < this->maxHeight; nLine++) {
        content = &this->contentLines[nLine];
        if (this->scrollX > 0) {
            if (this->contentLines[nLine].length() > this->scrollX)
                tmpWString = this->contentLines[nLine].substr(this->scrollX);
            else
                tmpWString = L"";
            content = &tmpWString;
        }
        renderer::drawString(content->c_str(), {this->pos.x + 1, this->pos.y + nLine - this->scrollY + 1}, this->size.x - 2);
        if (nLine - this->scrollY + 1 >= this->size.y - 2)
            break;
    }

    // == Draw VERTICAL scroll bar 
    if (_nbLines > _nbLinesDisplayable) {
        i2d _scrollBarVPos {this->pos.x + this->size.x - 1, this->pos.y + 1};
        int _scrollBarVHeight = static_cast<int>(std::ceil(_nbLinesDisplayable / _nbLines * _nbLinesDisplayable)) - 1;
        int _scrollBarVCursorY = _scrollBarVPos.y + static_cast<int>(std::ceil((this->scrollY / _nbLines) * _nbLinesDisplayable));

        for (uint16_t _y = _scrollBarVPos.y; _y < _scrollBarVPos.y + _nbLinesDisplayable; _y++) {
            if (_y >= _scrollBarVCursorY && _y < _scrollBarVCursorY + _scrollBarVHeight)
                renderer::drawString(globals::cursBlock, {_scrollBarVPos.x, _y});
            else
                renderer::drawString(globals::cursBlock3, {_scrollBarVPos.x, _y});
        }
    }

    // == Draw VERTICAL scroll bar 
    if (_nbCols > _nbColsDisplayable) {
        i2d _scrollBarHPos {this->pos.x + 1, this->pos.y + this->size.y - 1};
        int _scrollBarHHeight = static_cast<int>(std::ceil(_nbColsDisplayable / _nbCols * _nbColsDisplayable)) - 1;
        int _scrollBarHCursorX = _scrollBarHPos.x + static_cast<int>(std::ceil((this->scrollX / _nbCols) * _nbColsDisplayable));

        for (uint16_t _x = _scrollBarHPos.x; _x < _scrollBarHPos.x + _nbColsDisplayable; _x++) {
            if (_x >= _scrollBarHCursorX && _x < _scrollBarHCursorX + _scrollBarHHeight)
                renderer::drawString(globals::cursBlock, {_x, _scrollBarHPos.y});
            else
                renderer::drawString(globals::cursBlock3, {_x, _scrollBarHPos.y});
        }
    }
}
