#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleProgram.h"
#include "ModuleEngineCamera.h"

#include "SDL/include/SDL.h"
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

    SDL_PumpEvents();

    const Uint8* keyboard = SDL_GetKeyboardState(NULL);

    if (keyboard[SDL_SCANCODE_ESCAPE]) 
    {
        status = UPDATE_STOP;
    }
    if (keyboard[SDL_SCANCODE_LSHIFT])
    {
        App->engineCamera->Run(2.f);
    }
    if (keyboard[SDL_SCANCODE_W])
    {
        App->engineCamera->Move(camera_movement::MOVE_FORWARD);
    }
    if (keyboard[SDL_SCANCODE_S])
    {
        App->engineCamera->Move(camera_movement::MOVE_BACKWARDS);
    }
    if (keyboard[SDL_SCANCODE_Q])
    {
        App->engineCamera->Move(camera_movement::MOVE_UP);
    }
    if (keyboard[SDL_SCANCODE_E])
    {
        App->engineCamera->Move(camera_movement::MOVE_DOWN);
    }
    if (keyboard[SDL_SCANCODE_A])
    {
        App->engineCamera->Move(camera_movement::MOVE_LEFT);
    }
    if (keyboard[SDL_SCANCODE_D])
    {
        App->engineCamera->Move(camera_movement::MOVE_RIGHT);
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
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->renderer->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
                break;
        }
    }

    keyboard = SDL_GetKeyboardState(NULL);

    if (keyboard[SDL_SCANCODE_ESCAPE]) {
        status = UPDATE_STOP;
    }

    return status;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
    ENGINE_LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
