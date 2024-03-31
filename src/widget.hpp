#ifndef TERMTEMPLE_WIDGET_H
#define TERMTEMPLE_WIDGET_H

#include <string>
#include <vector>
#include "utils.hpp"

#define DEFAULT_OPENING_DELAY   250

class Module; // Forward declaration because Module also have a pointer to widget

class Widget {
protected:
    std::wstring title {};
    bool        bTitle {true};
    i2d         pos {0, 0};
    i2d         size {80, 25};
    uint8_t     colorPair {1}; // colorPairs::<COLOR>_ON_<COLOR>
    std::vector<Module*> modules {};
    long long   timeStart {0};
    long        timeOpeningMs {DEFAULT_OPENING_DELAY};
    bool        bIsOpening {true};
    long        timeLapsedMs {0};
    bool        bBorder {true};
    bool        bClosingCross {true};
    bool        bResizable {false};
    i2d         sizeMin {32, 10};
public:
    Widget(const std::wstring& _title, i2d _pos, i2d _size);
    explicit Widget(const std::wstring& _title);
    virtual ~Widget(); 

    virtual void update();
    virtual void draw();
    virtual void endOpening();
    virtual void handleKey(int _keycode);
    virtual void mainHandleKey(int _keycode);
    virtual void handleMouseClick(i2d _pos);
    void drawBorder(i2d _framePos, i2d _frameSize);
    void addModule(Module* _module);

    virtual void setPos(i2d _pos);
    virtual void setSize(i2d _size);
    void setColorPair(uint8_t _colorPair);
    void setBorder(bool _bBorder);
    void setTitle(const std::wstring& _title);
    void setResizable(bool _resizable) { this->bResizable = _resizable; }
    i2d  getPos() { return pos; }
    i2d  getSize() { return size; }
    i2d  getSizeMin() { return sizeMin; }
    long long getTimeStart() { return timeStart; }
    bool isResizable() { return bResizable; }
    //bool isOpening() { return (timeLapsedMs < timeOpeningMs); }
    bool isOpening() { return (bIsOpening); }
private:
    void drawFrame();
};

#endif