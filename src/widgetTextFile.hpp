#ifndef TERMTEMPLE_WIDGETTEXTFILE_H
#define TERMTEMPLE_WIDGETTEXTFILE_H

#include "widget.hpp"

class WidgetTextFile : public Widget {
protected:
    std::wifstream  txtFile {};
    std::vector<std::wstring> contentLines {};
    uint16_t        maxWidth {0}; // maximum size in character of a line
    uint16_t        maxHeight {0};
    uint16_t        scrollX {0};
    uint16_t        scrollY {0};
public:
    WidgetTextFile(const std::wstring& _title, const std::string& _filePath);
    virtual ~WidgetTextFile();
    virtual void draw() override;
    virtual void handleKey(int _keycode) override;
};

#endif
