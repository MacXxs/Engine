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

	GLuint Load(const char* fileName, const std::string filePath);

	int GetWidth() const;
	int GetHeight() const;

public:
	GLuint texture;

private:
	int width, height;
};

