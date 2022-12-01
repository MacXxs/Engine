#pragma once
#include "Module.h"
#include "Globals.h"

#include "Model.h"

#include <list>

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Start() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	void renderTriangle();

public:
	unsigned vbo;
	std::list<Model> models;
};