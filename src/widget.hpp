#ifndef TERMTEMPLE_WIDGET_H
#define TERMTEMPLE_WIDGET_H

#include <string>
#include <vector>
#include "utils.hpp"

class Module; // Forward declaration because Module also have a pointer to widget

class Widget {
protected:
    std::wstring title {};
    bool        bBorder {true};
    bool        bClosingCross {true};
    bool        bTitle {true};
    std::vector<Module*> modules;
    long long   timeStart {0};
    long        timeLapsedMs {0};
    i2d         pos {0, 0};
    i2d         size {80, 25};
    uint8_t     colorPair {0};
    bool        bResizable {false};
    i2d         sizeMin {32, 10};
public:
    Widget(const std::wstring& _title, i2d _pos, i2d _size);
    explicit Widget(const std::wstring& _title);
    virtual ~Widget(); 

    virtual void update();
    virtual void draw();
    virtual void handleKey(int _keycode);
    virtual void mainHandleKey(int _keycode);
    virtual void handleMouseClick(i2d _pos);
    void drawBorder();
    void addModule(Module* _module);

    virtual void setPos(i2d _pos);
    void setSize(i2d _size);
    void setColorPair(uint8_t _colorPair);
    void setBorder(bool _bBorder);
    void setTitle(const std::wstring& _title);
    i2d  getPos() { return this->pos; };
    i2d  getSize() { return this->size; };
    i2d  getSizeMin() { return this->sizeMin; };
    long long getTimeStart() { return this->timeStart; };
    bool isResizable () { return this->bResizable; };
};

#endif