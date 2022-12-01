#pragma once
#include "Module.h"
#include "Globals.h"

#include "SDL/include/SDL.h"

typedef unsigned __int8 Uint8;

enum class KeyState {
	IDLE,
	DOWN,
	UP,
	REPEAT
};

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

	KeyState GetKey(int scanCode) const;

private:
	const Uint8 *keyboard = NULL;

	KeyState keysState[SDL_NUM_SCANCODES] = { KeyState::IDLE };
};