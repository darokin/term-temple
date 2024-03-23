#include <map>
#include "utils.hpp"
#include "widgetsManager.hpp"
#include "widgetAppLauncher.hpp"
#include "globals.hpp"
#include "widget.hpp"
#include "screen.hpp"

#define APPLAUNCHER_NBAPPS  6
#define APPLAUNCHER_WIDTH   24
static const wchar_t* appLauncherTitle = L"__APPLAUNCHER__"; 
const std::wstring appNames[APPLAUNCHER_NBAPPS] = {L"TUTORIAL", L"SYSTEM", L"FILE EXPLORER", L"CLOCK", L"TODOS", L"HELP"};
//const wchar_t* appNames[NBAPPS] = {L"🔧 SYSTEM", L"🕑 CLOCK", L"📖 NOTES", L"🗒 TODOS"}; // ⌚ ⌛ 
// TODO : instead do a map where the key is the application name and the value is a pointer to the function

//static std::map<std::wstring, stAppElement> appMap = { { L"ABOUT", {&WidgetManager::openSystem} }, { L"TUTORIAL", {} }, { L"HELP", {WidgetManager::openHelp} } }; 

extern WidgetManager* wmgr;


WidgetAppLauncher::WidgetAppLauncher() : Widget(appLauncherTitle) {
    this->bTitle = false;
    this->size.x = APPLAUNCHER_WIDTH + 2;
    this->size.y = (APPLAUNCHER_NBAPPS * 1) + 2;
    this->pos.x = 4;
    this->pos.y = 4;
    /*
    uint8_t i {0};
    for (auto const& [key, val] : appMap) {
        const uint16_t nbSpacesBefore = (APPLAUNCHER_WIDTH - key.length()) / 2;
        const uint16_t nbSpacesAfter = APPLAUNCHER_WIDTH -key.length() - nbSpacesBefore;
        const std::wstring wsAppName = std::wstring(nbSpacesBefore, ' ') + key + std::wstring(nbSpacesAfter, ' ');
        ModuleOneLiner* tmpOL = new ModuleOneLiner(wsAppName, {1, (i * 1) + 1});
        i++;
        tmpOL->setWidget(this);
        tmpOL->updatePos();
        this->addModule(tmpOL);

        if (this->selectedModule == nullptr) {
            this->selectedModule = tmpOL;
            this->selectedModule->setColorReversed(true);
            this->selectedModuleIterator = appMap.begin();
        }
    }
    */
    for (int i = 0; i < APPLAUNCHER_NBAPPS; i++) {
        const uint16_t nbSpacesBefore = (APPLAUNCHER_WIDTH - appNames[i].length()) / 2;
        const uint16_t nbSpacesAfter = APPLAUNCHER_WIDTH - appNames[i].length() - nbSpacesBefore;
        const std::wstring wsAppName = std::wstring(nbSpacesBefore, ' ') + appNames[i] + std::wstring(nbSpacesAfter, ' ');
        ModuleOneLiner* tmpOL = new ModuleOneLiner(wsAppName, {1, (i * 1) + 1});
        tmpOL->setWidget(this);
        tmpOL->updatePos();
        this->addModule(tmpOL);

        if (this->selectedModule == nullptr) {
            this->selectedModule = tmpOL;
            this->selectedModule->setColorReversed(true);
            this->indSelected = i;
        }
    }
}

WidgetAppLauncher::~WidgetAppLauncher() {
}

void WidgetAppLauncher::handleKey(int _keycode) {
    bool bHasMoved {false};

    if (this->selectedModule == nullptr)
        return;

    switch (_keycode)
    {
        case KEY_UP:
            //if (this->selectedModuleIterator != appMap.begin())
            //    this->selectedModuleIterator--;
            if (this->indSelected > 0)
                this->indSelected--;
            bHasMoved = true;
            break;
        case KEY_DOWN:
            //if (this->selectedModuleIterator != appMap.end())
            //    this->selectedModuleIterator++;
            if (this->indSelected < APPLAUNCHER_NBAPPS - 1)
                this->indSelected++;
            bHasMoved = true;
            break;
        case KEY_SPACE:
        case KEY_ENTER:
        case KEY_ENTER_NPAD:
            //std::invoke(appMap[], wmgr);
            if (appNames[this->indSelected] == L"SYSTEM") {
                wmgr->openSystem();
            } else if (appNames[this->indSelected] == L"TUTORIAL") {
                wmgr->openTutorial();
            } else if (appNames[this->indSelected] == L"CLOCK") {
                wmgr->openClock();
            } else if (appNames[this->indSelected] == L"FILE EXPLORER") {
                wmgr->openFileExplorer();
            } else if (appNames[this->indSelected] == L"HELP") {
                wmgr->openHelp();
            } else {
                wmgr->alert(Utils::format_wstring(L"Application {} unavailable.", appNames[this->indSelected]));
            }
            break;
        default:
            break;
    }
    if (bHasMoved) {
        this->selectedModule->setColorReversed(false);
        this->selectedModule = dynamic_cast<ModuleOneLiner*>(this->modules.at(this->indSelected));//[this->indSelected];
        //this->selectedModule = this->selectedModuleIterator->second.module;
        this->selectedModule->setColorReversed(true);
        this->selectedModule->setTimeStart(globals::currentTimeInMs);
    }
}

void WidgetAppLauncher::draw() {
}
