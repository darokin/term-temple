#ifndef TERMTEMPLE_MODULE_H
#define TERMTEMPLE_MODULE_H

#include "utils.hpp"

class Widget; // forward declaration because pointing to each other

class Module {

// == Callback function for click event
typedef void (*triggerfunc)(Module*, Widget*, i2d);

protected:
    Widget* widget {nullptr};   // widget the module is linked to
    triggerfunc clickFunc {nullptr};
    i2d     initialPos {1, 1};  // initial position in the widget
    i2d     pos {1, 1};
    i2d     size {1, 1};
    uint8_t colorPair {0};
    bool    colorReversed {false};
    long long timeStart {0};
    long    timeLapsedMs {0};
    float   msPerChar {10.f};   // speed for the default animation of the module

public:
    Module(i2d _initialPos, i2d _size = {20, 8});
    virtual ~Module();
    
    virtual void draw();

    void update();
    void updatePos();
    void setWidget(Widget* _widget) { this->widget = _widget; };
    void setColorPair(uint8_t _colorPair) { this->colorPair = _colorPair; };
    void setColorReversed(bool _bReverse) { this->colorReversed = _bReverse; };
    void setSpeed(float _speed) { this->msPerChar = _speed; };
    void setPos(i2d _pos) { this->pos.x = _pos.x; this->pos.y = _pos.y; };
    void setInitialPos(i2d _initialPos) { this->initialPos = _initialPos; };
    void setSize(i2d _size) { this->size.x = _size.x; this->size.y = _size.y; };
    void setTimeStart(long long _timeStart) { this->timeStart = _timeStart; };
    void setTriggerClick(triggerfunc _clickFunc);
    i2d getPos() { return this->pos; };
    i2d getSize() { return this->size; };
    long long getTimeStart() { return this->timeStart; };
    triggerfunc getTriggerClick() { return this->clickFunc; };
};

#endif
