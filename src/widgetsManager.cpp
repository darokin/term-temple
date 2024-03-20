#include <stdexcept>
#include "globals.hpp"
#include "renderer.hpp"
#include "widgetsManager.hpp"
#include "widget.hpp"
#include "widgetDialog.hpp"
#include "widgetMsgbox.hpp"
#include "widgetANSI.hpp"
#include "widgetClock.hpp"
#include "widgetTextFile.hpp"
#include "widgetFileExplorer.hpp"
#include "moduleOneLiner.hpp"

extern wchar_t debugText[256];

WidgetManager* WidgetManager::widgetmgr = nullptr;

WidgetManager::WidgetManager() {
    // == Creation of APP LAUNCHER
    appLauncher = new WidgetAppLauncher();
    this->isOnAppLauncher = false;
    this->focusWidget = appLauncher;
    this->background = nullptr;
	// == We don't add it to the widgets because it is controlled individually
    // == Creation of STATUS module
    statusBar = new ModuleOneLiner(L"STATUS", {1, globals::termSize.y - 1});
    statusBar->setColorPair(globals::getColor(colorPairs::YELLOW_ON_BLUE));
}

WidgetManager* WidgetManager::getInstance() {
    if (widgetmgr == nullptr) {
        widgetmgr = new WidgetManager();
    }
    return widgetmgr;
}

void WidgetManager::draw() {
    // == Fill background
    // == Set Color
    renderer::setColor(globals::getColor(colorPairs::WHITE_ON_BLACK));
    // == Erase backgrounds with empty lines
    for (uint8_t _y = 0; _y < globals::termSize.y - 1; _y++) 
        renderer::drawString(globals::longSpacesLine, {0, _y}, globals::termSize.x);

    // == Draw background ANSI
    if (background != nullptr)
        background->draw();

    // == Draw widgets
    for (auto w : widgets) {
        w->update();
    }

    // == App launcher
    if (this->isOnAppLauncher)
        this->appLauncher->update(); 
}

void WidgetManager::setBackground(const std::string& _backgroundPath) {
    if (background != nullptr) {
        delete background;
    }

    i2d bkgSize;
    i2d tmpPos;

    background = new ModuleANSI(_backgroundPath);
    bkgSize = background->getSize();
    tmpPos.x = (globals::termSize.x - bkgSize.x) / 2.0;
    tmpPos.y = (globals::termSize.y - bkgSize.y) / 2.0;
    background->setPos(tmpPos);
}

void WidgetManager::handleKey(int _keycode) {
    if (_keycode == KEY_ESCAPE) {
        return;
    }
    if (_keycode == KEY_TAB) {
        if (this->widgets.size() <= 1)
            return;
        Widget* tmpWidget;
        tmpWidget = this->widgets.front();
        this->removeWidget(tmpWidget, false);
        this->addWidget(tmpWidget);
        return;
    }
    if (_keycode == 'q' || _keycode == 'Q') {
        if (this->focusWidget != nullptr)
            this->removeWidget(this->focusWidget);
        return;
    }
    if (this->focusWidget != nullptr)
        focusWidget->mainHandleKey(_keycode);
}

void WidgetManager::toggleAppLauncher() {
    this->isOnAppLauncher = !(this->isOnAppLauncher);
    if (this->isOnAppLauncher)
        this->focusWidget = this->appLauncher;
}

void WidgetManager::handleMove(int _keycode) {
    if (this->focusWidget == nullptr)
        return;
    i2d _pos = this->focusWidget->getPos();
    i2d _size = this->focusWidget->getSize();
    uint16_t _dec = 4;
    switch (_keycode) {
        case KEY_LEFT:
            _pos.x -= _dec;
            break;
        case KEY_RIGHT:
            _pos.x += _dec;
            break;
        case KEY_UP:
            _pos.y -= _dec;
            break;
        case KEY_DOWN:
            _pos.y += _dec;
            break;
    }
    if (_pos.x + _size.x > globals::termSize.x)
        _pos.x = globals::termSize.x - _size.x;
    if (_pos.x < 0)
        _pos.x = 0;
    if (_pos.y + _size.y > globals::termSize.y - 1)
        _pos.y = globals::termSize.y - _size.y - 1;
    if (_pos.y < 0)
        _pos.y = 0;
    focusWidget->setPos(_pos);
}

void WidgetManager::handleSnap(int _keycode) {
    if (this->focusWidget == nullptr)
        return;
    i2d _pos = this->focusWidget->getPos();
    i2d _size = this->focusWidget->getSize();
    switch (_keycode) {
        case KEY_LEFT:
            _pos.x = 0;
            break;
        case KEY_RIGHT:
             _pos.x = globals::termSize.x - _size.x;
            break;
        case KEY_UP:
            _pos.y = 0;
            break;
        case KEY_DOWN:
            _pos.y = globals::termSize.y - _size.y - 1;
            break;
    }
    focusWidget->setPos(_pos);
}

void WidgetManager::handleResize(int _keycode) {
    if (this->focusWidget == nullptr)
        return;
    if (!this->focusWidget->isResizable())
        return;
    i2d _pos { this->focusWidget->getPos() };
    i2d _size { this->focusWidget->getSize() };
    i2d _backupSize { _size };
    i2d _minSize { this->focusWidget->getSizeMin() };
    switch (_keycode) {
        case 'F':
        case 'f':
            if (_size.x == globals::termSize.x && _size.y == globals::termSize.y - 1) {
                _size.x = _minSize.x;
                _size.y = _minSize.y;
            } else {
                _pos.x = 0;
                _pos.y = 0;
                _size.x = globals::termSize.x;
                _size.y = globals::termSize.y - 1;
            }
            break;
        case KEY_LEFT:
            if (_size.x > _minSize.x)
                _size.x--;
            break;
        case KEY_RIGHT:
            if (_pos.x + _size.x + 1 < globals::termSize.x)
                _size.x++;
            break;
        case KEY_UP:
            if (_size.y > _minSize.y)
                _size.y--;
            break;
        case KEY_DOWN:
            if (_pos.y + _size.y + 1 < globals::termSize.y - 1)
                _size.y++;
            break;
    }
    if (_size != _backupSize) {
        focusWidget->setPos(_pos);
        focusWidget->setSize(_size);
        globals::setStatusText(L"[RESIZE] [%d, %d]", _size.x, _size.y);
    }
}

void WidgetManager::addWidget(Widget* _widget) {
    this->widgets.push_back(_widget);
    this->focusWidget = _widget;
    this->isOnAppLauncher = false;
}

void WidgetManager::removeWidget(Widget* _widget, bool bDoKill) {
    // == Remove widget from vector
    for (std::vector<Widget*>::iterator it = this->widgets.begin(); it != this->widgets.end(); ++it) {
        if (*it == _widget) {
            if (bDoKill)
                delete _widget;
            this->widgets.erase(it);
            break;
        }
    }
    // == Set focus on top widget OR the App Launcher if opened
    if (this->widgets.size() > 0)
        this->focusWidget = this->widgets.back();
    else if (this->isOnAppLauncher)
        this->focusWidget = this->appLauncher;
    else
        this->focusWidget = nullptr;
}

void WidgetManager::handleMousePressed(i2d _pos) {
    mousePressedFlag = true;
    mousePressedPos = _pos;
}

void WidgetManager::handleMouseReleased(i2d _pos) {
    mousePressedFlag = true;
    (void)_pos;
}

void WidgetManager::handleMouseMove(i2d _pos) {
    // == Only if pressed
    if (!mousePressedFlag)
        return;
    // == Only if it has moved
    if (_pos == mousePressedPos) 
        return;
    // == Widget move !
}

void WidgetManager::handleMouseClicked(i2d _pos) {
    Widget* _clickedWidget = nullptr;
    Widget* _w;
    i2d _wSize, _wPos;
    for (int i = widgets.size() - 1; i >= 0; i--) {
        _w = widgets[i];
        _wSize = _w->getSize();
        _wPos = _w->getPos();
        if (_pos.x >= _wPos.x && _pos.x < _wPos.x + _wSize.x \
        &&  _pos.y >= _wPos.y && _pos.y < _wPos.y + _wSize.y) {
        /* 
        if (_pos >= _wPos && _pos < _wPos + _wSize) {
        */
            _clickedWidget = _w;
            break;
        }
    }
    if (_clickedWidget != nullptr) {
        removeWidget(_clickedWidget, false);
        addWidget(_clickedWidget);
        this->focusWidget = _clickedWidget;
        _clickedWidget->handleMouseClick(_pos);
    }
}

void WidgetManager::alert(const std::wstring& _msg, bool _centered) {
    /*
    va_list vl;
    va_start(vl, _msg);
    size_t size = std::vswprintf(nullptr, 0, _msg.c_str(), vl);
    wchar_t _formatedMsg[size];
    vswprintf(_formatedMsg, size, _msg.c_str(), vl);
    va_end(vl);
    */
    WidgetMsgbox* newMsgbox = new WidgetMsgbox(_msg);
    newMsgbox->setBorder(true);
    newMsgbox->setTitle(L"WARNING");
    newMsgbox->setColorPair(colorPairs::BLACK_ON_YELLOW);
    newMsgbox->setCentered(_centered);
    addWidget(newMsgbox);
}

void WidgetManager::openFile(const std::string& _filePath) {
    Widget* _newWidget;
    std::string _extension {};
    std::string _fileName {};

    // == Get filename 
    _fileName = _filePath.substr(_filePath.find_last_of('/') + 1);

    // == Get Extension
    if (_fileName.length() > 4) 
        _extension = _fileName.substr(_fileName.length() - 4, 4);

    // == Images
    if (_extension == ".ans") {
        _newWidget = new WidgetANSI(Utils::str2wstr(_fileName), _filePath);
        _newWidget->setPos({1, 1});
    } else {
        // == File without extension
        _newWidget = new WidgetTextFile(Utils::str2wstr(_fileName), _filePath);
        _newWidget->setColorPair(colorPairs::WHITE_ON_BLACK);
        _newWidget->setPos({12, 5});
        _newWidget->setSize({52, 30});
    }

    addWidget(_newWidget);
}

void WidgetManager::openHelp() {
    i2d _size {64, 24};
    i2d _pos {(globals::termSize.x - _size.x) / 2, (globals::termSize.y - _size.y) / 2 - 1};
    WidgetTextFile* _wHelp;

    _wHelp = new WidgetTextFile(L"HELP", "../data/txt/help.txt");
    _wHelp->setColorPair(colorPairs::BLUE_ON_WHITE);
    _wHelp->setPos(_pos);
    _wHelp->setSize(_size);
    addWidget(_wHelp);
}

void WidgetManager::openSystem() {
    WidgetANSI* _wANSI = new WidgetANSI(L" SYSTEM ", "../data/ans/system01.ans");
    _wANSI->setPos({8, 6});
    addWidget(_wANSI);
}

void WidgetManager::openTutorial() {
    WidgetDialog* _wDialog = WidgetDialog::createDialogPtrWithFile("../data/txt/tutorial.txt");
    _wDialog->setTitle(L" TUTORIAL ");
    addWidget(_wDialog);
}

void WidgetManager::openFileExplorer() {
    WidgetFileExplorer* _wFileExplorer = new WidgetFileExplorer();
    addWidget(_wFileExplorer);
}

void WidgetManager::openClock() {
    WidgetClock* _wClock = new WidgetClock(L"CLOCK", {12, 10}, {48, 12});
    _wClock->setColorPair(colorPairs::BLUE_ON_YELLOW);
    addWidget(_wClock);
}

// TODO : is it useless
// TODO : instead of blind centering, calculate widget that are out of bounds
// and move them inside the screen. Display alert if screensize too small ?
void WidgetManager::refreshWidgets(uint16_t _termCols, uint16_t _termLines) {
    i2d tmpPos{};
    i2d bkgSize {};

    // == Calculate size and set position of the background image
    bkgSize = background->getSize();
    tmpPos.x = (_termCols - bkgSize.x) / 2.0;
    tmpPos.y = (_termLines - bkgSize.y) / 2.0;
    background->setPos(tmpPos);

    // == Update widgets 
    i2d wSize;
    for (auto w : widgets) {
        // == Centering
        wSize = w->getSize();
        tmpPos.x = (_termCols - wSize.x) / 2.0;
        tmpPos.y = (_termLines - wSize.y) / 2.0;

        w->setPos(tmpPos);
    }
}

void WidgetManager::freeWidgets() {
    for (auto w : widgets) {
        delete w;
    }
    if (background)
        delete background; // background ModuleANSI
    if (appLauncher)
        delete appLauncher; // widgetAppLauncher
}

