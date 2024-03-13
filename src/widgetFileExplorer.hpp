#ifndef TERMTEMPLE_WIDGETFILEEXPLORER_H
#define TERMTEMPLE_WIDGETFILEEXPLORER_H

#include <map>
#include "moduleOneLiner.hpp"
#include "widget.hpp"

//typedef struct stFile;
struct stFile {
    stFile* parent {nullptr};
    std::wstring name;
    std::wstring parentName;
    uint16_t yPosition;
    bool bIsOpen {false};
    bool bIsFile {false};
    ModuleOneLiner* module {nullptr};
};

class WidgetFileExplorer : public Widget {
protected:
    stFile* selectedFile {nullptr}; // == Pointer on selected file.
    std::multimap<std::wstring, stFile*> files; // == Map of files (key is path, val is entry name)
    std::vector<stFile*> filesDisplayed; // == Vector of all displayed files currently
    uint16_t nbLines {0};
    uint16_t nbLinesDisplayable {0};
    uint16_t scrollY {0};

    ModuleOneLiner* pathModule;
    ModuleOneLiner* lineModule;
public:
    WidgetFileExplorer();
    virtual ~WidgetFileExplorer();
    virtual void draw() final;
    virtual void handleKey(int _keycode) final;
    virtual void setPos(i2d _pos) final;
    void addFilesToTree(const std::wstring& _path);
    void recursiveScanFolder(const std::string& _filesPath);
    void calcTree();
    void drawTree();
    void drawScrollbar();
};

#endif
