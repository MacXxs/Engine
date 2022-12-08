#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleDebugDraw.h"
#include "ModuleEditor.h"
#include "ModuleEngineCamera.h"
#include "ModuleTexture.h"

using namespace std;

constexpr int FRAMES_BUFFER = 50;

Application::Application()
{
	fpsLog = std::vector<float>(FRAMES_BUFFER, 0.0f);
	msLog = std::vector<float>(FRAMES_BUFFER, 0.0f);

	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(input = new ModuleInput());
	modules.push_back(program = new ModuleProgram()); 
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(debug = new ModuleDebugDraw());
	modules.push_back(textures = new ModuleTexture());
	modules.push_back(engineCamera = new ModuleEngineCamera());

	appTimer = new Timer;
	maxFramerate = MAX_FRAMERATE;
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

bool Application::Start()
{
	bool ret = true;

	appTimer->Start();

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Start();

	return ret;
}

update_status Application::Update()
{
	float ms = appTimer->Read();

	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	float dt = (appTimer->Read() - ms) / 1000.0f;

	if (dt < 1000.0f / GetMaxFrameRate())
	{
		SDL_Delay(1000.0f / GetMaxFrameRate() - dt);
	}

	this->deltaTime = (appTimer->Read() - ms) / 1000.0f;
	this->fps = 1 / this->deltaTime;
	this->AddFrame(App->fps, App->deltaTime);

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	delete appTimer;

	return ret;
}

void Application::AddFrame(int fps, float ms)
{
	for (unsigned i = 0; i < FRAMES_BUFFER - 1; ++i)
	{
		fpsLog[i] = fpsLog[i + 1];
		msLog[i] = msLog[i + 1];
	}

	fpsLog[FRAMES_BUFFER - 1] = fps;
	msLog[FRAMES_BUFFER - 1] = ms * 1000.0f;
}

void Application::SetMaxFrameRate(int maxFrames)
{
	this->maxFramerate = maxFrames;
}

int Application::GetMaxFrameRate() const
{
	return this->maxFramerate;
}
