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
class ModuleTexture;

class Application
{
public:

	Application();
	~Application();

	bool Start();
	bool Init();
	update_status Update();
	bool CleanUp();

	void AddFrame(int fps, float ms);

	void SetMaxFrameRate(int maxFrames);
	int GetMaxFrameRate() const;

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleProgram* program = nullptr;
	ModuleRenderExercise* object = nullptr;
	ModuleDebugDraw* debug = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleEngineCamera* engineCamera = nullptr;
	ModuleTexture* textures = nullptr;

	float deltaTime = 0.f;
	int fps = 0;

	std::vector<float> fpsLog;
	std::vector<float> msLog;

private:
	std::list<Module*> modules;

	int maxFramerate = MAX_FRAMERATE;
};

extern Application* App;
