#include <string>
#include <map>
#include <iostream>
#include <filesystem>
#include "utils.hpp"
#include "globals.hpp"
#include "screen.hpp"
#include "renderer.hpp"
#include "widget.hpp"
#include "widgetsManager.hpp"
#include "widgetFileExplorer.hpp"
#include "moduleOneLiner.hpp"

static const wchar_t* fileExplorerTitle = L"__FILEEXPLORER__"; 
static const std::string fakePathToDataFiles = "/home/darokin/code/cpp/term-temple/";
extern WidgetManager* wmgr;

/*
#ifdef _WIN32
static const std::multimap<std::wstring, std::wstring> filesList {{L"C:\\Users\\rebuzzi\\Documents\\PERSO\\", L"code\\"}};
#else
*/
static const std::multimap<std::wstring, std::wstring> filesList {{L"/", L"usr/"}, {L"/", L"home/"}, {L"/", L"sys/"}, \
                                                            {L"/usr/", L"bin/"}, {L"/usr/", L"lib/"}, {L"/usr/", L"include/"}, {L"/usr/", L"share/"}, {L"/usr/", L"sbin/"}, \
                                                                    {L"/usr/share/", L"blender/"}, {L"/usr/share/", L"blender/"}, {L"/usr/share/", L"cmake/"}, \
                                                                     {L"/usr/share/", L"tools/"},  {L"/usr/share/", L"themes/"},
                                                            {L"/home/", L"darokin/"}, \
                                                               {L"/home/darokin/", L"code/"}, {L"/home/darokin/", L"documents/"}, {L"/home/darokin/", L"games/"}, {L"/home/darokin/", L"images/"}, \
                                                                    {L"/home/darokin/games/", L"steam/"}, \
                                                                    {L"/home/darokin/code/", L"c/"}, {L"/home/darokin/code/", L"cpp/"}, {L"/home/darokin/code/", L"python/"}, \
                                                            {L"/sys/", L"dev/"}, {L"/sys/", L"module/"}};
//#endif 

WidgetFileExplorer::WidgetFileExplorer() : Widget(fileExplorerTitle) {
    this->bTitle = false;
    // == Init File Explorer Widget
    this->size.x = 54;//60;
    this->size.y = 24;//32;
    this->pos.x = 8;
    this->pos.y = 3;
    this->nbLinesDisplayable = this->size.y - 4; // 2 for borders and 2 for the address header
    
    // == Initializing the multimap of stFiles where the key is the path and the value is a stFile
    stFile* tmpFile;
    for (auto const& [key, val] : filesList) {
        tmpFile = new stFile();
        tmpFile->name = val;
        tmpFile->parentName = key;
        tmpFile->bIsOpen = false;
        tmpFile->yPosition = 0;
        tmpFile->bIsFile = (val.back() != '/');
        this->files.insert({key, tmpFile});
    }

    // == Scan the 'files' folder
    //#ifdef _WIN32
    //    std::string _filesPath = "C:\\Users\\rebuzzi\\Documents\\PERSO\\code\\";
    //#else
        //std::string _curDirPath = std::filesystem::current_path().string();
        //std::string _filesPath { _curDirPath.substr(0, _curDirPath.size() - 3) + "data" }; //"../data/"; //
        std::string _filesPath { "/home/darokin/code/cpp/" };
        recursiveScanFolder(_filesPath);
    //#endif
    

    // == Add header module (address bar)
    // == Add PATH module
    pathModule = new ModuleOneLiner(Utils::str2wstr(_filesPath).substr(0, this->size.x - 2), {1, 1});
    pathModule->setWidget(this);
    pathModule->updatePos();
    
    // == Add separator line
    std::wstring sLine = std::wstring(this->size.x, L'═');
    sLine[sLine.size() - 1] = L'╣';
    sLine[0] = L'╠';
    lineModule = new ModuleOneLiner(sLine, {0, 2});
    lineModule->setWidget(this);
    lineModule->updatePos();
    
    // == Calculate first tree and add to display the visible modules / files
    calcTree();
    drawTree();

    // == Change path in address bar
    this->pathModule->setText(this->selectedFile->parentName + this->selectedFile->name);
}

WidgetFileExplorer::~WidgetFileExplorer() {
}

/*
 * RECURSIVESCANFOLDER scan a folder and add all entries to stFile
 * Add each entry to the 'files' multimap
 * Recursively call it again if the entry is a folder
 */
/*
void WidgetFileExplorer::recursiveScanFolder(std::string _filesPath) {
    stFile* _tmpFile;
    std::string _strPath;
    std::string _strParentPath;

    // == We build a correct non relative path for the parent
    //_strParentPath = _filesPath;
    if (_filesPath.find("../") != std::string::npos)
        _strParentPath = fakePathToDataFiles + _filesPath.substr(3);
    else
        _strParentPath = _filesPath;

    // == Go throught directory_iterator
    for (const auto& _fileName : std::filesystem::directory_iterator(_filesPath)) {
        _strPath = _fileName.path().u8string();
        // == We replace '../' to have a non relative path that fit in the directory structured we created
        if (_strPath.find("../") != std::string::npos)
            _strPath.replace(0, 3, fakePathToDataFiles);

        // == Storing the file
        _tmpFile = new stFile();
        if (_fileName.is_directory())
            _tmpFile->name = Utils::str2wstr(_strPath.substr(_strPath.find_last_of('/', -1) + 1) + "/");
        else
            _tmpFile->name = Utils::str2wstr(_strPath.substr(_strPath.find_last_of('/') + 1));
        //_tmpFile->parentName = Utils::str2wstr(_filesPath);
        _tmpFile->parentName = Utils::str2wstr(_strParentPath);
        _tmpFile->bIsOpen = false;
        _tmpFile->yPosition = 0;
        _tmpFile->bIsFile = !_fileName.is_directory();
        //this->files.insert({Utils::str2wstr(_filesPath), _tmpFile});
        this->files.insert({Utils::str2wstr(_strParentPath), _tmpFile});

        // == Do recursion if its a directory
        if (_fileName.is_directory()) {
            std::string pathForRecursion { _fileName.path().u8string() + "/" };
            recursiveScanFolder(pathForRecursion);//_fileName.path().u8string() + "/");
        }
    }
}
*/

void WidgetFileExplorer::recursiveScanFolder(const std::string& _filesPath) {
    stFile* tmpFile;

    for (const auto& _fileName : std::filesystem::directory_iterator(_filesPath)) {
        tmpFile = new stFile();
        if (_fileName.is_directory())
            tmpFile->name = Utils::str2wstr(_fileName.path().u8string().substr(_fileName.path().u8string().find_last_of('/', -1) + 1) + "/");
        else
            tmpFile->name = Utils::str2wstr(_fileName.path().u8string().substr(_fileName.path().u8string().find_last_of('/') + 1));
        tmpFile->parentName = Utils::str2wstr(_filesPath);
        tmpFile->bIsOpen = false;
        tmpFile->yPosition = 0;
        tmpFile->bIsFile = !_fileName.is_directory();
        this->files.insert({Utils::str2wstr(_filesPath), tmpFile});

        if (_fileName.is_directory())
            recursiveScanFolder(_fileName.path().u8string() + "/");
    }
}

/*
 * RECALC TREE only takes the list of stFiles
 * and populate them with the  
 */
void WidgetFileExplorer::calcTree() {
    // == Clear file displayed
    filesDisplayed.clear();

    // == Reset nbLines and call recursive addModules()
    nbLines = 0;
    this->addFilesToTree(L"/");
    nbLines--;
}

/*
 * REDRAW clear all the lines (modules) of the Explorer and generate them again
 * according to whether they are open or not (via the above recursive ADD FILES MODULES funtion)
 * it also counts the number of lines displayed and store it to nbLines (TODO : why not modules.length() - 2 ?)
 */
void WidgetFileExplorer::drawTree() {
    std::wstring sPath;
    ModuleOneLiner* _lineModule;
    size_t _nbSlashes;
    std::wstring _spacing;
    std::wstring _icon;

    // == Clear modules
    for (auto m : modules) {
        delete m;
    }
    modules.clear();
/*
    // == Set path with selected stFile
    if (this->selectedFile == nullptr)
        sPath = L"/";
    else
        sPath = this->selectedFile->parentName + this->selectedFile->name;
*/

    // ======================================================
    // == Display modules that are visible
    for (const auto& _stFile : this->filesDisplayed) {
        if (_stFile->yPosition < scrollY)
            continue;
        if (_stFile->yPosition >= this->nbLinesDisplayable + scrollY)
            break;
        
        // == Calculate depth 
        _nbSlashes = std::count_if(_stFile->parentName.begin(), _stFile->parentName.end(), []( wchar_t wc ){ return wc == L'/'; });
        _nbSlashes--; // So we are at 0 slashes for root
        _spacing = std::wstring((_nbSlashes * 2), ' ');

        // == Setup an icon
        #ifdef _WIN32
            _icon = (_stFile->bIsFile ? L"F" : L"D");
            _icon = (_stFile->bIsOpen ? L"O" : _icon);
        #else
            _icon = (_stFile->bIsFile ? L"🗎" : L"🗀");
            _icon = (_stFile->bIsOpen ? L"🗁" : _icon);
        #endif

        // == Create a new module at the correct line and add it to the modules of the widget
        i2d _modulePos {1, (_stFile->yPosition * 1) - this->scrollY + (this->size.y - this->nbLinesDisplayable - 1)};
        _lineModule = new ModuleOneLiner(_spacing + _icon + L" " + _stFile->name, _modulePos);
        _lineModule->setWidget(this);
        _lineModule->updatePos();
        _lineModule->setTimeStart(_lineModule->getTimeStart() - 99999);
        if (this->selectedFile == _stFile)
            _lineModule->setColorReversed(true);
        this->addModule(_lineModule);
        _stFile->module = _lineModule; 
    }

    // == Display scrollbar if necessary
    this->drawScrollbar();
}

void WidgetFileExplorer::addFilesToTree(const std::wstring& _path) {
    stFile* _stFile;
    auto _itResults = this->files.equal_range(_path); // find all results in multimap for that key

    // == Iterating through results of stFiles with that key / path
    for (auto _it = _itResults.first; _it != _itResults.second; ++_it) {
        _stFile = _it->second; // For easy access

        // == For initialisation purpose, select first stFile at start
        if (this->selectedFile == nullptr)
            this->selectedFile = _stFile;

        // == Store in the stFile its position and the module it is associated to 
        _stFile->yPosition = this->nbLines;
        
        // == Store the stFile to be displayed later
        this->filesDisplayed.push_back(_stFile);

        // == Counting number of lines
        this->nbLines++;

        // == Recursive call for an opened folder
        // == We call addFiles for the new key which is the current key + the folder name
        if (_stFile->bIsOpen && !(_stFile->bIsFile))
            this->addFilesToTree(_path + _stFile->name);
    }
}

void WidgetFileExplorer::handleKey(int _keycode) {
    bool bHasMoved {false};
    bool bHasOpenedClosed {false};
    bool bHasScrolled {false};
    uint16_t prevScrollY {this->scrollY};

    if (this->selectedFile == nullptr)
        return;

    // TODO replace 10 par PAGE_SKIP ds les cases NPAGE PPAGE

    uint16_t _yPos = this->selectedFile->yPosition;

    switch (_keycode) {
        case KEY_UP:
            if (_yPos != 0) {
                if (_yPos - this->scrollY < 3 && this->scrollY > 0) {
                    bHasScrolled = true;
                    this->scrollY--;
                }
                _yPos--;
                bHasMoved = true;
            }
            break;
        case KEY_DOWN:
            if (_yPos < this->nbLines) {
                if (_yPos - this->scrollY > this->nbLinesDisplayable - 4) {
                    bHasScrolled = true;
                    this->scrollY++;
                }
                _yPos++;
                bHasMoved = true;
            }
            break;
        case KEY_SPACE:
        case KEY_ENTER:
            if (this->selectedFile->bIsFile) {
                // == Opening FILES
                wmgr->openFile(Utils::wstr2str(this->selectedFile->parentName + this->selectedFile->name));
            } else {
                // == Opening or closing FOLDERS
                this->selectedFile->bIsOpen = !this->selectedFile->bIsOpen;
                bHasOpenedClosed = true;
            }
            break;
        case KEY_RIGHT:
            // == Do nothing if it's an opened folder or a file
            if (this->selectedFile->bIsOpen || this->selectedFile->bIsFile)
                break;
            this->selectedFile->bIsOpen = true;
            bHasOpenedClosed = true;
            break;
        case KEY_LEFT:
            // == Do nothing if it's a closed folder or a file
            if (!this->selectedFile->bIsOpen || this->selectedFile->bIsFile)
                break;
            this->selectedFile->bIsOpen = false;
            bHasOpenedClosed = true;
            break;
        case KEY_PPAGE:
            if (_yPos > 0) {
                if (_yPos > 10)
                    _yPos -= 10;
                else
                    _yPos = 0;
                bHasMoved = true;

                if (_yPos < this->scrollY + 2) {
                    this->scrollY = std::max(0, _yPos - 2);
                    if (this->scrollY != prevScrollY)
                        bHasScrolled = true;
                }                
            }
            break;
        case KEY_NPAGE:
            if (_yPos < this->nbLines) {
                if (_yPos + 10 < this->nbLines)
                    _yPos += 10;
                else
                    _yPos = this->nbLines;
                bHasMoved = true;

                if (_yPos > this->scrollY + this->nbLinesDisplayable - 2) {
                    this->scrollY = _yPos - this->nbLinesDisplayable + 3; 
                    bHasScrolled = true;
                }
            }
            break;
        default:
            break;
    }
    if (bHasMoved) {
        for (auto const& val : this->filesDisplayed) {
            // == Find new selected stFile 
            // == Deselect current selected stFile module
            // == Store the new selected stFile
            // == Select new stFile module
            if (val->yPosition == _yPos && this->selectedFile->module != nullptr) {
                this->selectedFile->module->setColorReversed(false);   
                this->selectedFile = val;
                // == We drawTree() here so the new selected stFile has a module loaded (because it is drawn)
                if (bHasScrolled)
                    this->drawTree();
                this->selectedFile->module->setColorReversed(true);   
                break;             
            }
        }
        // == Change path in address bar
        this->pathModule->setText((this->selectedFile->parentName + this->selectedFile->name).substr(0, this->size.x - 2));
    }
    if (bHasOpenedClosed) {
        this->calcTree();
        this->drawTree();
    }
}

void WidgetFileExplorer::drawScrollbar() {
    float _nbLines = static_cast<float>(this->nbLines);
    float _nbLinesDisplayable = static_cast<float>(this->nbLinesDisplayable);
    float _scrollbarMaxHeight = static_cast<float>(this->size.y - 2);

    // == Draw the scroll bar 
    if (_nbLines <= _nbLinesDisplayable) 
        return;
        
    i2d _scrollBarPos {this->pos.x + this->size.x - 1, this->pos.y + 1};
    int _scrollBarHeight = static_cast<int>(std::ceil(_nbLinesDisplayable / _nbLines * _scrollbarMaxHeight)) - 1;
    int _scrollBarCursorY = _scrollBarPos.y + static_cast<int>(std::ceil((this->scrollY / _nbLines) * _scrollbarMaxHeight));
    for (uint16_t _y = _scrollBarPos.y; _y < _scrollBarPos.y + _scrollbarMaxHeight; _y++) {
        if (_y >= _scrollBarCursorY && _y < _scrollBarCursorY + _scrollBarHeight)
            renderer::drawString(globals::cursBlock, {_scrollBarPos.x, _y});
        else
            renderer::drawString(globals::cursBlock3, {_scrollBarPos.x, _y});
    }
}

void WidgetFileExplorer::setPos(i2d _pos) {
    this->pos = _pos;
    this->pathModule->updatePos();
    this->lineModule->updatePos();
    for (const auto& m : modules)
        m->updatePos();
}

void WidgetFileExplorer::draw() {
    // everything is in the widget modules
    this->pathModule->mainDraw();
    this->lineModule->mainDraw();
    this->drawScrollbar();
}
