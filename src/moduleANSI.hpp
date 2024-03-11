#ifndef TERMTEMPLE_MODULEANSI_H
#define TERMTEMPLE_MODULEANSI_H

#include "module.hpp"

class ModuleANSI : public Module {
private:
    std::ifstream ansiFile;
public:
    ModuleANSI(const std::string& _filePath, i2d _pos, i2d _size);
    explicit ModuleANSI(const std::string& _filePath);
    virtual ~ModuleANSI();
    virtual void draw();
};

#endif