#ifndef TERMTEMPLE_MODULEONELINER_H
#define TERMTEMPLE_MODULEONELINER_H

#include "module.hpp"
#include "utils.hpp"

class ModuleOneLiner: public Module {
private:
    std::wstring line;
    int nbChars;
    bool centered {false};
    bool bIsDone {false};
public:
    ModuleOneLiner(const std::wstring& _line, i2d _pos);
    virtual ~ModuleOneLiner() {};
    void setText(const std::wstring& _line);
    void draw();
    bool isDone() { return this->bIsDone; };
    void setCentered(bool _centered) { this->centered = _centered; };
};

#endif
