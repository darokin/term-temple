#include "moduleDialog.hpp"
#include "renderer.hpp"
#include "globals.hpp"

int endWordInHowManyChar(std::wstring wstr, size_t wordCountStart) {
    size_t _strLenght = wstr.length();
    if (wordCountStart >= _strLenght)
        return 0;
    for (size_t i = wordCountStart; i < _strLenght; i++) {
        if (wstr[i] == L' ' || wstr[i] == L'\n')
            return i - wordCountStart;
    }
    return _strLenght - wordCountStart;
}

ModuleDialog::ModuleDialog(i2d _pos, uint16_t _nbColumns, uint16_t _lineMaxForced) 
    : Module(_pos, {_nbColumns, _lineMaxForced}), nbColumns {_nbColumns}, lineMaxForced {_lineMaxForced} {
    this->seqTimeStart = this->timeStart;
}

ModuleDialog::ModuleDialog(i2d _pos, uint16_t _nbColumns) 
    : ModuleDialog(_pos, _nbColumns, 0) {
}

void ModuleDialog::draw() {
    uint16_t nbCharsToShow {0};
    uint16_t nbCharsToShowTotal {0};
    uint16_t x {0};
    uint16_t y {(uint16_t)this->pos.y};
    uint16_t xdec {0}; // dec if centered
    uint16_t slen {0}; // current string length
    // == Security
    if (widget == nullptr)
        return;
    // == For moduleDialog, elapsedMs is made with seqStart and not timeStart
    this->timeLapsedMs = globals::currentTimeInMs - this->seqTimeStart;
    // == Calculate nb chars visible
    nbCharsToShow = this->timeLapsedMs / this->msPerChar;
    if (nbCharsToShow > this->sequences[this->seqIndex]->nbChars)
        nbCharsToShow = this->sequences[this->seqIndex]->nbChars;
    nbCharsToShowTotal = nbCharsToShow;
    // == Placing characters
    for (auto s : this->sequences[this->seqIndex]->lines) {
        slen = wcslen(s);
        xdec = (this->centered ? (this->nbColumns - slen) / 2 : 0);
        if (wcslen(s) > nbCharsToShow) {
            renderer::drawString(s, {this->pos.x + xdec, y}, nbCharsToShow);
            x = nbCharsToShow + this->pos.x + xdec;
            y++;
            break;    
        }
        renderer::drawString(s, {this->pos.x + xdec, y});
        nbCharsToShow -= slen;
        x = this->pos.x + xdec + slen - 1; 
        y++;
    }
    // == Test if current dialog sequence completely done
    this->bIsDone = false;
    if (nbCharsToShowTotal >= this->sequences[this->seqIndex]->nbChars)
        this->bIsDone = true;
    // == Blink cursor while printing
    if (this->blink && !this->bIsDone)
        renderer::drawString(((this->timeLapsedMs % 2) ? globals::cursBlock : L"_"), {x, y - 1}, nbCharsToShow);
}

void ModuleDialog::dialogPrev() {
    long _timeCurrent = globals::currentTimeInMs;
    this->seqTimeStart = _timeCurrent;
    this->bIsDone = false;
    if (this->seqIndex > 0)
        this->seqIndex--;
}

void ModuleDialog::dialogNext() {
    uint16_t nbCharsToShow;

    nbCharsToShow = this->timeLapsedMs / this->msPerChar;
    if (nbCharsToShow < this->sequences[this->seqIndex]->nbChars) {
        this->seqTimeStart -= this->sequences[this->seqIndex]->nbChars * this->msPerChar;
        this->bIsDone = true;
        return;
    }

    if (this->seqIndex >= this->sequences.size() - 1)
        return;
     
    this->seqTimeStart = globals::currentTimeInMs;
    this->bIsDone = false;
    this->seqIndex++;
}

void ModuleDialog::setText(std::wstring _line) {
    std::wistringstream _wis(_line);
    std::wstring _ws;
    std::vector<std::wstring> _lines;

    while (std::getline(_wis, _ws))
        _lines.push_back(_ws);

    setText(_lines);
}

void ModuleDialog::setText(std::vector<std::wstring>& _lines) {
    dialogSequence* newDialogSeq;
    wchar_t* newLine;
    uint16_t lineCount {0};
    uint16_t charCount {0};
    uint16_t endWord;
    wchar_t wc;
    const wchar_t seqCharSeparator {L'-'};
    
    newDialogSeq = new dialogSequence{ {}, 0, 0 };
    this->sequences.push_back(newDialogSeq);

    newLine = new wchar_t[this->nbColumns+1];
    for (size_t i = 0; i < _lines.size(); i++) {
        // == Test if on a dialog separator 
        if (_lines[i].length() < 3 && _lines[i][0] == seqCharSeparator) { 
            sequences.back()->nbLines = lineCount;
            if (lineCount > lineMax)
              lineMax = lineCount;
            lineCount = 0;
            newDialogSeq = new dialogSequence{ {}, 0, 0 };
            sequences.push_back(newDialogSeq);
            continue;
        }
        // == Empty line
        if (_lines[i].length() <= 0) {
            newLine[0] = L'\0';
            sequences.back()->lines.push_back(newLine);
            sequences.back()->nbChars++;
            newLine = new wchar_t[nbColumns+1];
            lineCount++;
        }
        for (size_t ichar = 0; ichar < _lines[i].length(); ichar++) {
            wc = _lines[i][ichar];
            // == Skip unlucky space at the beginning of the line jump
            if (charCount == 0 && ichar > 0 && wc == L' ' && _lines[i].length() > 1)
              continue;
            // == Copy char by char
            newLine[charCount] = wc;
            charCount++;
            // == Test saut de ligne 
            endWord = endWordInHowManyChar(_lines[i], ichar+1);//charCount);
            if ((endWord + charCount > nbColumns && wc == L' ') || charCount >= nbColumns || ichar == _lines[i].length()-1) {
                // == Closing line, pushing it and starting a new one
                newLine[charCount] = L'\0';
                sequences.back()->lines.push_back(newLine);
                sequences.back()->nbChars += charCount;
                newLine = new wchar_t[nbColumns+1];
                lineCount++;
                charCount = 0;
            }
        }
        // == Test si on dépasse le nb de line max autorisée
        if (this->lineMaxForced > 0 && sequences.back()->lines.size() > 0 && sequences.back()->lines.size() % this->lineMaxForced == 0) {
            lineCount = 0;
            newDialogSeq = new dialogSequence{ {}, 0, 0 };
            sequences.push_back(newDialogSeq);
        }
    }

    // == Fin de la dernière dialogSequence 
    this->sequences.back()->nbLines = lineCount;
    if (lineCount > lineMax)
        this->lineMax = lineCount;

    this->size.y = this->lineMax;
    
    this->bIsDone = false;
}
