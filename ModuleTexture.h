#pragma once
#include "Module.h"

#include "GL/glew.h"

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	bool Start();

	update_status Update();

	GLuint Load(const char* fileName);

public:
	GLuint texture;
};

