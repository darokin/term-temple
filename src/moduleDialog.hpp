#ifndef TERMTEMPLE_MODULEDIALOG_H
#define TERMTEMPLE_MODULEDIALOG_H

#include <vector>
#include <sstream>

#include "module.hpp"
#include "utils.hpp"

struct dialogSequence {
    std::vector<wchar_t*> lines;
    int nbLines;
    int nbChars;
};

class ModuleDialog: public Module {
private:
    std::vector<dialogSequence*> sequences{};
    size_t      seqIndex {0};
    long long   seqTimeStart {0};
    uint16_t    nbColumns;
    uint16_t    lineMax {0};
    uint16_t    lineMaxForced {0};
    bool        blink {true};
    bool        centered {false};
    bool        bIsDone {false};
public:
    ModuleDialog(i2d _pos, uint16_t _nbColumns, uint16_t _lineMaxForced);
    ModuleDialog(i2d _pos, uint16_t _nbColumns);
    ~ModuleDialog() {};
    void draw();
    void dialogPrev();
    void dialogNext();
    void setText(std::vector<std::wstring>& _lines);
    void setText(const std::wstring& _line);
    void setCentered(bool _centered) { this->centered = _centered; };
    void setLineMaxForced(uint16_t _lineMaxForced) { this->lineMaxForced = _lineMaxForced; };
    uint16_t getLineMax() { return this->lineMax; };
    uint16_t getLineMaxForced() { return lineMaxForced; };
    bool isDone() { return this->bIsDone; };
    bool isLastSequence() { return (seqIndex == sequences.size() - 1); };
};

#endif
