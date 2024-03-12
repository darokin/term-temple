#ifndef TERMTEMPLE_WIDGETANSI_H
#define TERMTEMPLE_WIDGETANSI_H

#include "widget.hpp"

class WidgetANSI : public Widget {
private:
    std::ifstream ansiFile;
public:
    WidgetANSI(const std::wstring& _title, i2d _pos, i2d _size);
    explicit WidgetANSI(const std::wstring& _title);
    WidgetANSI(const std::wstring& _title, const char* _ansiFilePath);
    virtual ~WidgetANSI();
    void draw();
};

#endif