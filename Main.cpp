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

int main(int argc, char ** argv)
{
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			ENGINGE_LOG("Application Creation --------------");
			App = new Application();
			state = MAIN_INIT;
			break;

		case MAIN_INIT:
			ENGINGE_LOG("Application Init --------------");
			if (App->Init() == false)
			{
				ENGINGE_LOG("Application Init exits with error -----");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_START;
				ENGINGE_LOG("Application Update --------------");
			}

			break;

		case MAIN_START:

			ENGINGE_LOG("Application Start --------------");
			if (App->Start() == false)
			{
				ENGINGE_LOG("Application Init exits with error -----");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				ENGINGE_LOG("Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				ENGINGE_LOG("Application Update exits with error -----");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			ENGINGE_LOG("Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				ENGINGE_LOG("Application CleanUp exits with error -----");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}

	}

	delete App;
	ENGINGE_LOG("Bye :)\n");
	return main_return;
}
