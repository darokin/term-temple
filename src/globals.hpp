#ifndef TERMTEMPLE_GLOBALS_H
#define TERMTEMPLE_GLOBALS_H

#include "utils.hpp"

enum class GameState {
	BOOTLOADING,
	INGAME_START,
	INGAME_MOVE,
	INGAME_RESIZE,
	INGAME_SNAP,
	INGAME_END,
	INANIMATION
};

namespace globals {
    extern const wchar_t* cursBlock;
    extern const wchar_t* cursBlock2;
    extern const wchar_t* cursBlock3;
	extern const wchar_t* longSpacesLine;
	extern long long currentTimeInMs;
    void tick();
	int setStatusText(const wchar_t* _line, ...);
	const wchar_t* getStatusText();

	extern GameState gameState;
};

#endif