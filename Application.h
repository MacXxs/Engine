#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleInput;
class ModuleProgram;
class ModuleRenderExercise;
class ModuleDebugDraw;
class ModuleEditor;
class ModuleEngineCamera;

class Application
{
public:

	Application();
	~Application();

	bool Start();
	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleProgram* program = nullptr;
	ModuleRenderExercise* object = nullptr;
	ModuleDebugDraw* debug = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleEngineCamera* engineCamera = nullptr;

private:

	std::list<Module*> modules;

};

extern Application* App;
