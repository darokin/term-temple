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
    ModuleButton(const std::wstring& _wstr, i2d _pos, uint16_t _nbColumns);
    ModuleButton(const std::wstring& _wstr, i2d _pos);
    ModuleButton(const ModuleButton& _mbutton); // no need... (TODO)
    virtual ~ModuleButton() {};
    virtual void draw();
    void setText(const std::wstring& _line);
};

#endif
