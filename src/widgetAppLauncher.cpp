#include "utils.hpp"
#include "globals.hpp"
#include "widget.hpp"
#include "moduleOneLiner.hpp"
#include "widgetsManager.hpp"
#include "widgetAppLauncher.hpp"

#include "screen.hpp"

#define APPLAUNCHER_NBAPPS  6
#define APPLAUNCHER_WIDTH   24
static const wchar_t* appLauncherTitle = L"__APPLAUNCHER__"; 
//const wchar_t* appNames[NBAPPS] = {L"🔧 SYSTEM", L"🕑 CLOCK", L"📖 NOTES", L"🗒 TODOS"};
//const wchar_t* appNames[APPLAUNCHER_NBAPPS] = {L"SYSTEM", L"CLOCK", L"NOTES", L"TODOS"};
const std::wstring appNames[APPLAUNCHER_NBAPPS] = {L"SYSTEM", L"FILE EXPLORER", L"CLOCK", L"NOTES", L"TODOS", L"HELP"};
// ⌚ ⌛ 

extern WidgetManager* wmgr;

WidgetAppLauncher::WidgetAppLauncher() : Widget(appLauncherTitle) {
    this->bTitle = false;
    this->size.x = APPLAUNCHER_WIDTH + 2;
    this->size.y = (APPLAUNCHER_NBAPPS * 1) + 2;
    this->pos.x = 4;
    this->pos.y = 4;

    ModuleOneLiner* tmpOL;
    uint16_t nbSpacesBefore, nbSpacesAfter;
    std::wstring wsAppName;
    for (int i = 0; i < APPLAUNCHER_NBAPPS; i++) {
        nbSpacesBefore = (APPLAUNCHER_WIDTH - appNames[i].length()) / 2;
        nbSpacesAfter = APPLAUNCHER_WIDTH - appNames[i].length() - nbSpacesBefore;
        wsAppName = std::wstring(nbSpacesBefore, ' ') + appNames[i] + std::wstring(nbSpacesAfter, ' ');
        tmpOL = new ModuleOneLiner(wsAppName.c_str(), {1, (i * 1) + 1});
        tmpOL->setWidget(this);
        tmpOL->updatePos();
        this->addModule(tmpOL);

        if (this->selectedModule == nullptr) {
            this->selectedModule = tmpOL;
            this->indSelected = i;
            this->selectedModule->setColorReversed(true);
        }
    } 
    /*
    this->indSelected = 0;
    this->selector = new ModuleOneLiner(std::wstring(maxX, '_'), {1, (this->indSelected * 2) + 2});
    this->selector->setSpeed(30);
    this->selector->setWidget(this);
    this->selector->updatePos();
    */
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
            if (this->indSelected > 0)
                this->indSelected--;
            bHasMoved = true;
            break;
        case KEY_DOWN:
            if (this->indSelected < APPLAUNCHER_NBAPPS - 1)
                this->indSelected++;
            bHasMoved = true;
            break;
        case KEY_SPACE:
        case KEY_ENTER:
            if (appNames[this->indSelected] == L"SYSTEM") {
                wmgr->openSystem();
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
        this->selectedModule->setColorReversed(true);
        this->selectedModule->setTimeStart(globals::currentTimeInMs);
    }
}

/* void WidgetAppLauncher::setPos(i2d _pos) {
    this->pos = _pos;
    selector->updatePos();
}
 */
void WidgetAppLauncher::draw() {
    //this->selector->mainDraw();
}
