#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleProgram.h"
#include "ModuleEngineCamera.h"

#include "Libraries/ImGui/imgui_impl_sdl.h"

#include <GL/glew.h>

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
    ENGINE_LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
        ENGINE_LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
    update_status status = UPDATE_CONTINUE;
    
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
        if (keysState[i] == KeyState::DOWN)
            keysState[i] = KeyState::REPEAT;

        if (keysState[i] == KeyState::UP)
            keysState[i] = KeyState::IDLE;
    }

    SDL_PumpEvents();

    const Uint8* keyboard = SDL_GetKeyboardState(NULL);

    if (keyboard[SDL_SCANCODE_ESCAPE]) 
    {
        status = UPDATE_STOP;
    }

    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

        switch (sdlEvent.type)
        {
        case SDL_QUIT:
            return UPDATE_STOP;

        case SDL_WINDOWEVENT:
            if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED ||
                sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                App->renderer->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);

            break;

        case SDL_KEYDOWN:
            this->keysState[sdlEvent.key.keysym.scancode] = KeyState::DOWN;
            break;

        case SDL_KEYUP:
            this->keysState[sdlEvent.key.keysym.scancode] = KeyState::UP;
            break;

        case SDL_MOUSEWHEEL:
            if (sdlEvent.wheel.y > 0)
            {
                App->engineCamera->Run();
                App->engineCamera->Move();
            }
            else if (sdlEvent.wheel.y < 0)
            {
                App->engineCamera->Run();
                App->engineCamera->Move();
            }

            break;
        }

        if (sdlEvent.type == SDL_MOUSEMOTION && sdlEvent.motion.state & SDL_BUTTON_RMASK)
        {
            App->engineCamera->MouseRotate(sdlEvent.motion.xrel, sdlEvent.motion.yrel);
        }

    }

    return status;
}

KeyState ModuleInput::GetKey(int scanCode) const
{
    return keysState[scanCode];
}

// Called before quitting
bool ModuleInput::CleanUp()
{
    ENGINE_LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
