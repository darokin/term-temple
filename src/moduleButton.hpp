#ifndef TERMTEMPLE_MODULEBUTTON_H
#define TERMTEMPLE_MODULEBUTTON_H

#include "module.hpp"
#include "widget.hpp"
#include "utils.hpp"

class ModuleButton: public Module {
private:
    std::wstring    sText;
    uint16_t        nbColumns;
public:
    ModuleButton(std::wstring _wstr, i2d _pos, uint16_t _nbColumns);
    ModuleButton(std::wstring _wstr, i2d _pos);
    virtual ~ModuleButton() {};
    virtual void draw();
    void setText(std::wstring _line);
};

#endif
