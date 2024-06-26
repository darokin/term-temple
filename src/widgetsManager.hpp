#ifndef TERMTEMPLE_WIDGETMANAGER_H
#define TERMTEMPLE_WIDGETMANAGER_H

#include <vector>
#include "widget.hpp"
#include "widgetAppLauncher.hpp"
#include "module.hpp"
#include "moduleOneLiner.hpp"
#include "moduleANSI.hpp"

#define STATUS_TEXT_MAXLENGTH   (256)

class WidgetManager {
protected:
    WidgetManager(); 
    static WidgetManager* widgetmgr;
    std::vector<Widget*> widgets;
    Widget* focusWidget {nullptr};
    // TODO : check : ces 3 variables après on été add jeu 29 fev alors que tout était clean
    // recheck si pas mieux dans screen ou entre screen et wmgrd
    ModuleANSI* background {nullptr};
    WidgetAppLauncher* appLauncher {nullptr};
    bool isOnAppLauncher {false};
    ModuleOneLiner* statusBar {nullptr};
    wchar_t statusText[STATUS_TEXT_MAXLENGTH] {};
    // == Handle mouse
    bool mousePressedFlag {false};
    i2d mousePressedPos {0, 0};
public:
    static WidgetManager* getInstance();
    void draw();
    void addWidget(Widget* _widget);
    void closeWidget(Widget* _widget);
    void refreshWidgets(uint16_t _termCols, uint16_t _termLines);
    void handleKey(int _keycode);
    void handleMove(int _keycode);
    void handleSnap(int _keycode);
    void handleResize(int _keycode);
    void handleMouseClicked(i2d _pos);
    void handleMousePressed(i2d _pos);
    void handleMouseReleased(i2d _pos);
    void handleMouseMove(i2d _pos);
    void freeWidgets();
    void toggleAppLauncher();
    void setBackground(const std::string& _backgroundPath);
    void setStatusText(const wchar_t* _line, ...);

    void openSystem();
    void openTutorial();
    void openHelp();
    void openClock();
    void openFileExplorer();
    void openFile(const std::string& _filePath);
    void alert(const std::wstring& _msg, bool _centered = true);
private:
    void removeWidget(Widget* _widget);
    void drawStatusBar();
    Widget* nextFocusWidget();
};

#endif