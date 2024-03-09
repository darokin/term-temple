#ifndef TERMTEMPLE_MODULESCROLLER_H
#define TERMTEMPLE_MODULESCROLLER_H

#include "module.hpp"
#include "utils.hpp"

class ModuleScroller: public Module {
private:
    std::wstring sText;
    uint16_t nbColumns;
    bool centered { false };
public:
    ModuleScroller(i2d _pos, uint16_t _nbColumns);
    virtual ~ModuleScroller() {};
    void setText(std::wstring _line);
    void setSpeed(long _ms);
    virtual void draw();
    void setCentered(bool _centered) { this->centered = _centered; };
};

#endif
