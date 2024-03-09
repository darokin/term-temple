#include "module.hpp"
#include "moduleANSI.hpp"
#include "cursesAnsi.hpp"

ModuleANSI::ModuleANSI(std::string _filePath, i2d _pos, i2d _size) : Module::Module(_pos, _size) {
    ansiFile.open(_filePath);
    if (!ansiFile.is_open())
        std::cout << "Unable to open ANSI file '" << _filePath << "'" << std::endl; 

    // == Detect size
    /*
    std::string _line;
    int _h {0}, _w {0}, _lineLength {0};
    if (_size.x == 0 && _size.y == 0) {
        while (getline(this->ansiFile, _line)) {
            _lineLength = cursesANSI::getANSIstrLength(_line); 
            if (_lineLength > _w)
                _w = _lineLength;
            _h++;
        }
        this->size.x = _w;
        this->size.y = _h;

    }
    */
        this->size.x = 80;
        this->size.y = 26;
}

ModuleANSI::ModuleANSI(std::string _filePath) : ModuleANSI::ModuleANSI(_filePath, {0, 0}, {0, 0}) {
}

ModuleANSI::~ModuleANSI() {
    // == ansiFile is ifstream (auto closing)
}

void ModuleANSI::draw() {
    std::string _line;
    int y{ 0 };
    
    if (!ansiFile.is_open())
        return;

    this->ansiFile.clear();
    this->ansiFile.seekg(0);
    while (getline(this->ansiFile, _line) && y < this->size.y) {
        if (!cursesANSI::mvwaddnANSIstr(this->pos.y + y, this->pos.x + 1, _line))
            break;
        y++;
    }
}
