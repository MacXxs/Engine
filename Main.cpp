#include <stdlib.h>
#include "Application.h"
#include "ModuleRender.h"
#include "Globals.h"

#include "SDL/include/SDL.h"

#pragma comment( lib, "SDL/lib/x64/SDL2.lib" )
#pragma comment( lib, "SDL/lib/x64/SDL2main.lib" )

enum main_states
{
	MAIN_CREATION,  
	MAIN_START,
	MAIN_INIT,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = NULL;
EngineLog* engineLog = new EngineLog();

int main(int argc, char ** argv)
{
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	Uint32 oldTick = SDL_GetTicks();
	Uint32 newTick;

	while (state != MAIN_EXIT)
	{
		newTick = SDL_GetTicks();
		oldTick = newTick;

		switch (state)
		{
		case MAIN_CREATION:

			ENGINE_LOG("Application Creation --------------");
			App = new Application();
			state = MAIN_INIT;
			break;

		case MAIN_INIT:
			ENGINE_LOG("Application Init --------------");
			if (App->Init() == false)
			{
				ENGINE_LOG("Application Init exits with error -----");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_START;
				ENGINE_LOG("Application Update --------------");
			}

			break;

		case MAIN_START:

			ENGINE_LOG("Application Start --------------");
			if (App->Start() == false)
			{
				ENGINE_LOG("Application Start exits with error -----");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				ENGINE_LOG("Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				ENGINE_LOG("Application Update exits with error -----");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			ENGINE_LOG("Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				ENGINE_LOG("Application CleanUp exits with error -----");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;
		}

		// Capping framerate and storing it
		float dt = (float)(SDL_GetTicks() - oldTick) / 1000.0f;
		
		if (dt < (1000.0f / MAX_FRAMERATE))
		{
			SDL_Delay((1000.0f / MAX_FRAMERATE) - dt);
		}
		
		App->deltaTime = (float)(SDL_GetTicks() - oldTick) / 1000.0f;
		App->fps = 1 / App->deltaTime;
		App->AddFrame(App->fps);
	}

	delete App;
	ENGINE_LOG("Bye :)\n");
	return main_return;
}
